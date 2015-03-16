// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/16/2015
// Description: Implements a parser for language resource files and dumps the data into sql files.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================

#include "../utils/gender.hpp"
#include "../utils/numerus.hpp"
#include "../utils/helper.hpp"
#include "../utils/exception.hpp"
#include "../utils/command_line_exception.hpp"

#include <cstring>
#include <cstddef>
#include <iostream>
#include <string>
#include <stdexcept>
#include <regex>
#include <list>
#include <stack>
#include <utility>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::regex;
using std::smatch;
using std::regex_match;
using std::regex_search;
using std::strcmp;

using lgeorgieff::translate::utils::Gender;
using lgeorgieff::translate::utils::Numerus;
using lgeorgieff::translate::utils::trim;
using lgeorgieff::translate::utils::normalize_whitespace;
using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::CommandLineException;

typedef std::list<string> strings;

bool STRICT_MODE = false;

// Regular expressions for gender.
const regex REGEX_GENDER_M{"\\{ *m *\\}"};
const regex REGEX_GENDER_F{"\\{ *f *\\}"};
const regex REGEX_GENDER_N{"\\{ *n *\\}"};

// Regular expressions for numerus.
const regex REGEX_NUMERUS_PL{"\\{ *pl\\.? *\\}"};
const regex REGEX_NUMERUS_SG{"\\{ *sg\\.? *\\}"};

// Regular expressions for abbreviations and comments
const regex REGEX_ABBREVIATION{"\\<.*\\>"};
const regex REGEX_COMMENT{"\\[.*\\]"};

// A struct that helds all data for a language item
struct LangItem {
  LangItem() = default;
  LangItem(const LangItem &) = default;
  LangItem(LangItem &&) = default;
  ~LangItem() = default;
  LangItem &operator=(const LangItem &) = default;
  LangItem &operator=(LangItem &&) = default;

  string language;
  string phrase;
  strings word_classes;
  strings comments;
  strings abbreviations;
  Gender gender;
  Numerus numerus;
};

// Find and replace a regular epxression in string by another string.
bool find_and_replace(string &str, const regex &find, const string &replacement) {
  smatch m;
  if (!regex_search(str, m, find)) return false;

  for (const string &matched_part : m) str.replace(str.find(matched_part), matched_part.size(), replacement);
  return true;
}

// Search for a gender part in a language entry. If one is found, the corresponding gender instance is returned and
// the matched gender string is removed from the original string.
Gender process_gender(string &entry) {
  if (find_and_replace(entry, REGEX_GENDER_M, ""))
    return Gender::m;
  else if (find_and_replace(entry, REGEX_GENDER_F, ""))
    return Gender::f;
  else if (find_and_replace(entry, REGEX_GENDER_N, ""))
    return Gender::n;
  else
    return Gender::none;
}

// Search for a numerus part in a language entry. If one is found, the corresponding numerus instance is returned and
// the matched numerus string is removed from the original string.
Numerus process_numerus(string &entry) {
  if (find_and_replace(entry, REGEX_NUMERUS_PL, ""))
    return Numerus::pl;
  else if (find_and_replace(entry, REGEX_NUMERUS_SG, ""))
    return Numerus::sg;
  else
    return Numerus::none;
}

// A compare function for a filtered comment string
bool compare_match_items(const std::pair<size_t, string> &lft, const std::pair<size_t, string> &rgt) {
  return lft.first <= rgt.first;
}

// Search for a closed substring in between a start and an end tag. The substring is found by the passed regular
// expression. If one substring is found, the corresponding string value is returned and the matched substring is
// removed from the original string.
strings process_closed_substring(string &entry, size_t line_number, const regex &main_rex, char opening_tag,
                                 char closing_tag) {
  strings result;
  smatch rex_match;
  if (!regex_search(entry, rex_match, main_rex)) return result;
  for (const string &comment : rex_match) {
    entry.replace(entry.find(comment), comment.size(), "");
    std::list<std::pair<size_t, string>> current_match_results;
    std::stack<std::pair<size_t, string>> collector;
    for (size_t pos{0}; pos != comment.size(); ++pos) {
      if (closing_tag == comment[pos] && collector.empty()) {
        // handle bad entry, one enclosing tag too much, e.g. ']'
        if (STRICT_MODE)
          throw Exception(string{"Bad comment syntax, found a \"" + string{closing_tag} + "\" too much in line " +
                                 std::to_string(line_number) + "!"});
        continue;
      } else if (closing_tag == comment[pos]) {
        normalize_whitespace(collector.top().second);
        if (!collector.top().second.empty()) current_match_results.push_back(collector.top());
        collector.pop();
      } else if (opening_tag == comment[pos]) {
        collector.push(std::pair<size_t, string>{pos, ""});
      } else if (!collector.empty()) {
        collector.top().second += comment[pos];
      }
    }
    // handle bad entries, too less enclosing tags, e.g. "]"
    if (!collector.empty() && STRICT_MODE)
      throw Exception(string{"Bad comment syntax, found a \"" + string{closing_tag} + "\" too few in line " +
                             std::to_string(line_number) + "!"});
    while (!collector.empty()) {
      normalize_whitespace(collector.top().second);
      if (!collector.top().second.empty()) current_match_results.push_back(collector.top());
      collector.pop();
    }
    current_match_results.sort(compare_match_items);
    for (const std::pair<size_t, string> &item : current_match_results) result.push_back(item.second);
  }
  return result;
}

// Search for an abbreviation part in a language entry. If one is found, the corresponding string value is returned and
// the matched abbreviation string is removed from the original string.
strings process_abbreviations(string &entry, size_t line_number) {
  return process_closed_substring(entry, line_number, REGEX_ABBREVIATION, '<', '>');
}

// Search for an comment part in a language entry. If one is found, the corresponding string value is returned and
// the matched comment string is removed from the original string.
strings process_comments(string &entry, size_t line_number) {
  return process_closed_substring(entry, line_number, REGEX_COMMENT, '[', ']');
}

// Process a phrase entry, i.e. normalize whitespace.
string process_phrase(string &entry) {
  normalize_whitespace(entry);
  return entry;
}

// Escapes the sinple apostrophe (') to ('').
void escape_apostrophe(string &str) {
  for (size_t pos{0}; pos < str.size(); ++pos) {
    if (str[pos] == '\'') str.insert(pos++, "'");
  }
}

// Process a lang item, i.e. all charactersitics of a language item.
LangItem process_lang(string &entry, const strings &word_classes, const string &language, size_t line_number) {
  LangItem item;
  item.language = language;
  item.word_classes = word_classes;
  item.gender = process_gender(entry);
  item.numerus = process_numerus(entry);
  item.abbreviations = process_abbreviations(entry, line_number);
  for (string &abbr : item.abbreviations) escape_apostrophe(abbr);
  item.comments = process_comments(entry, line_number);
  for (string &comment : item.comments) escape_apostrophe(comment);
  item.phrase = process_phrase(entry);
  escape_apostrophe(item.phrase);
  return item;
}

// Return a string representing gender that can be directly passed into an SQL query string.
string gender_to_sql_string(const Gender &gender, bool is_where = false) {
  string gender_str{to_db_string(gender)};
  if (is_where && Gender::none == gender) gender_str.insert(0, "is ");
  else if (is_where) gender_str.insert(0, "=");
  return gender_str;
}

// Return a string representing numerus that can be directly passed into an SQL query string.
string numerus_to_sql_string(const Numerus &numerus, bool is_where = false) {
  string numerus_str{to_db_string(numerus)};
  if (is_where && Numerus::none == numerus) numerus_str.insert(0, "is ");
  else if (is_where) numerus_str.insert(0, "=");
  return numerus_str;
}

// Create all required SQL statements for representing an entry of a language resource for a particular language.
void language_to_sql_statement(const LangItem &lang) {
  string gender_str{gender_to_sql_string(lang.gender)};
  string numerus_str{numerus_to_sql_string(lang.numerus)};
  string gender_where_str{gender_to_sql_string(lang.gender, true)};
  string numerus_where_str{numerus_to_sql_string(lang.numerus, true)};

  cout << "INSERT INTO phrase (phrase, language, gender, numerus) SELECT '" << lang.phrase << "', '" << lang.language
       << "', " << gender_str << ", " << numerus_str << " WHERE NOT EXISTS (SELECT 1 FROM phrase WHERE phrase='"
       << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str << " and numerus "
       << numerus_where_str << ");" << endl;

  for (const string &word_class : lang.word_classes) {
    cout << "INSERT INTO phrase_word_class (phrase_id, word_class_id) SELECT (SELECT id FROM PHRASE WHERE phrase='"
         << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str << "), '" << word_class
         << "' WHERE NOT EXISTS (SELECT 1 FROM phrase_word_class WHERE phrase_id=(SELECT id FROM PHRASE WHERE phrase='"
         << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str << ") and word_class_id='" << word_class << "');" << endl;
  }

  for (const string &comment : lang.comments) {
    cout << "INSERT INTO comment (comment) SELECT '" << comment
         << "' WHERE NOT EXISTS (SELECT 1 FROM comment WHERE comment='" << comment << "');" << endl;
    cout << "INSERT INTO phrase_comment (phrase_id, comment_id) SELECT (SELECT id FROM phrase WHERE phrase='"
         << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str << "), (SELECT id FROM comment WHERE comment='" << comment
         << "') WHERE NOT EXISTS (SELECT 1 FROM phrase_comment WHERE phrase_id=(SELECT id FROM phrase WHERE phrase='"
         << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str << ") and comment_id=(SELECT id FROM comment WHERE comment='"
         << comment << "'));" << endl;
  }

  for (const string &abbreviation : lang.abbreviations) {
    cout << "INSERT INTO abbreviation (abbreviation) SELECT '" << abbreviation
         << "' WHERE NOT EXISTS (SELECT 1 FROM abbreviation WHERE abbreviation='" << abbreviation << "');" << endl;
    cout << "INSERT INTO phrase_abbreviation (phrase_id, abbreviation_id) SELECT (SELECT id FROM phrase WHERE phrase='"
         << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str << "), (SELECT id FROM abbreviation WHERE abbreviation='"
         << abbreviation
         << "') WHERE NOT EXISTS (SELECT 1 FROM phrase_abbreviation WHERE phrase_id=(SELECT id FROM phrase WHERE "
            "phrase='" << lang.phrase << "' and language='" << lang.language << "' and gender " << gender_where_str
         << " and numerus " << numerus_where_str
         << ") and abbreviation_id=(SELECT id FROM abbreviation WHERE abbreviation='" << abbreviation << "'));"
         << endl;
  }
}

// Transform a language item to an SQL statement, so that it can be inserted to a DB.
void line_to_sql_statement(const LangItem &l_1, const LangItem &l_2) {
  language_to_sql_statement(l_1);
  language_to_sql_statement(l_2);
  string gender_str_1{gender_to_sql_string(l_1.gender, true)};
  string numerus_str_1{numerus_to_sql_string(l_1.numerus, true)};
  string gender_str_2{gender_to_sql_string(l_2.gender, true)};
  string numerus_str_2{numerus_to_sql_string(l_2.numerus, true)};

  cout << "INSERT INTO phrase_translation (phrase_id_in, phrase_id_out) SELECT (SELECT id FROM phrase WHERE(phrase='"
       << l_1.phrase << "' and language='" << l_1.language << "' and gender " << gender_str_1 << " and numerus "
       << numerus_str_1 << ")), (SELECT id FROM phrase WHERE(phrase='" << l_2.phrase << "' and language='"
       << l_2.language << "' and gender " << gender_str_2 << " and numerus " << numerus_str_2
       << ")) WHERE NOT EXISTS (SELECT 1 FROM phrase_translation WHERE phrase_id_in=(SELECT id FROM phrase "
          "WHERE(phrase='" << l_1.phrase << "' and language='" << l_1.language << "' and gender " << gender_str_1
       << " and numerus " << numerus_str_1 << ")) and phrase_id_out=(SELECT id FROM phrase WHERE(phrase='"
       << l_2.phrase << "' and language='" << l_2.language << "' and gender " << gender_str_2 << " and numerus "
       << numerus_str_2 << ")));" << endl;

  cout << endl;
}

// Process all word classes from the passed string and return a list with an item for each word class
strings get_word_classes(string word_classes_string) {
  strings result;
  string current_word_class;
  for (const char &c : word_classes_string) {
    if (c != '-' && !isalpha(c)) {
      trim(current_word_class);
      result.push_back(current_word_class);
      current_word_class.clear();
    } else {
      current_word_class.insert(current_word_class.end(), c);
    }
  }
  trim(current_word_class);
  if (!current_word_class.empty()) result.push_back(current_word_class);
  return result;
}

// Process a line from a language resorource, i.e. parse both languages in the passed line and create for each language
// an SQL statement.
void process_line(size_t line_number, const string &line, const string &lang_id_1, const string &lang_id_2) {
  size_t delimiter_lang{line.find('\t')};
  if ((string::npos == delimiter_lang || delimiter_lang + 1 == line.size()) && STRICT_MODE) {
    throw std::out_of_range("Line " + std::to_string(line_number) +
                            " in language resource does not contain a tab representing a delimiter between two" +
                            " languages!");
  }
  size_t delimiter_class{line.find('\t', delimiter_lang + 1)};
  string lang_entry_1{line.substr(0, delimiter_lang)};
  string lang_entry_2{line.substr(delimiter_lang + 1, delimiter_class - delimiter_lang - 1)};
  strings word_classes;
  if (string::npos != delimiter_class) word_classes = get_word_classes(line.substr(delimiter_class + 1, string::npos));

  LangItem lang_item_1 = process_lang(lang_entry_1, word_classes, lang_id_1, line_number);
  LangItem lang_item_2 = process_lang(lang_entry_2, word_classes, lang_id_2, line_number);

  if ((lang_item_1.phrase.empty() || lang_item_2.phrase.empty()) && STRICT_MODE)
    throw Exception(string{"No translation found in line " + std::to_string(line_number) + "!"});
  else if (!lang_item_1.phrase.empty() && !lang_item_2.phrase.empty())
    line_to_sql_statement(lang_item_1, lang_item_2);
}

void print_usage(string self_name, std::ostream &destination) {
  destination << endl;
  destination << "usage: " << self_name << "--in lang_1 --out lang_2 [--strict-mode]" << endl;
  destination << endl;
  destination << "--in | -i <language id 1>    Set the language identifier of the" << endl;
  destination << "                             source language, e.g. EN, DE" << endl;
  destination << "--out | -o <language id 2>   Set the language identifier of the" << endl;
  destination << "                             target language, e.g. EN, DE" << endl;
  destination << "--strict-mode | -s           Set the parser to strict mode, i.e." << endl;
  destination << "                             every parser error causes a run" << endl;
  destination << "                             to abort" << endl;
  destination << "--help | -h                  Shows this dialog and exits this programme" << endl;
}

// The entry point for this programme
int main(const int argc, const char **argv) {
  string lang_id_1;
  string lang_id_2;
  string error_message;
  for(int pos{1}; argc > pos; ++pos) {
    if((!strcmp("--in", argv[pos]) || !strcmp("-i", argv[pos]))){
      if (argc - 1 != pos) lang_id_1 = argv[++pos];
    } else if ((!strcmp("--out", argv[pos]) || !strcmp("-o", argv[pos]))) {
      if (argc - 1 != pos) lang_id_2 = argv[++pos];
    } else if (!strcmp("--strict-mode", argv[pos]) || !strcmp("-s", argv[pos])) {
      STRICT_MODE = true;
    } else if (!strcmp("--help", argv[pos]) || !strcmp("-h", argv[pos])) {
      print_usage(argv[0], cout);
      return 0;
    } else {
      if(error_message.empty())
	cerr << "The option \"" << argv[pos] << "\" is unknown!" << endl;
      else
	cerr << error_message << endl;
      cerr << "Formore help run \"" << argv[0] << " -h\"" << endl;
      return 2;
    }
  }
  if(lang_id_1.empty() || lang_id_2.empty()) {
    cerr << "The value for \"--in\" | \"-i\" and \"--out\" | \"-o\" must not be empty!" << endl;
    cerr << "Formore help run \"" << argv[0] << " -h\"" << endl;
    return 1;
  }

  string line;
  size_t line_counter{0};
  while (std::getline(std::cin, line)) process_line(++line_counter, line, lang_id_1, lang_id_2);

  if(!std::cin.eof() || std::cin.bad()) {
    cerr << "Failed to read from stdin!" << endl;
    return 1;
  } 

  return 0;
}
