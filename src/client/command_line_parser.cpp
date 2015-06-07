// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/07/2015
// Description: Implements the command line parser interface which is adapted to the supported options of this program.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
// the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program; if
// not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
// ====================================================================================================================

#include "command_line_parser.hpp"
#include "../utils/command_line_exception.hpp"

#include <cstring>
#include <utility>

namespace lgeorgieff {
namespace translate {

using lgeorgieff::translate::utils::CommandLineException;

const std::string CommandLineParser::IN_NAME_LONG{"--in"};
const std::string CommandLineParser::IN_NAME_SHORT{"-i"};
const std::string CommandLineParser::OUT_NAME_LONG{"--out"};
const std::string CommandLineParser::OUT_NAME_SHORT{"-o"};
const std::string CommandLineParser::HELP_NAME_LONG{"--help"};
const std::string CommandLineParser::HELP_NAME_SHORT{"-h"};
const std::string CommandLineParser::ALL_LANGUAGES_LONG{"--all-languages"};
const std::string CommandLineParser::LANGUAGE_ID_LONG{"--language-id"};
const std::string CommandLineParser::LANGUAGE_NAME_LONG{"--language-name"};
const std::string CommandLineParser::ALL_WORD_CLASSES_LONG{"--all-word-classes"};
const std::string CommandLineParser::WORD_CLASS_ID_LONG{"--word-class-id"};
const std::string CommandLineParser::WORD_CLASS_NAME_LONG{"--word-class-name"};
const std::string CommandLineParser::ALL_GENDERS_LONG{"--all-genders"};
const std::string CommandLineParser::GENDER_ID_LONG{"--gender-id"};
const std::string CommandLineParser::GENDER_NAME_LONG{"--gender-name"};
const std::string CommandLineParser::ALL_NUMERI_LONG{"--all-numeri"};
const std::string CommandLineParser::SHOW_PHRASE_LONG{"--show-phrase"};
const std::string CommandLineParser::SHOW_WORD_CLASS_LONG{"--show-word-class"};
const std::string CommandLineParser::SHOW_GENDER_LONG{"--show-gender"};
const std::string CommandLineParser::SHOW_NUMERUS_LONG{"--show-numerus"};
const std::string CommandLineParser::SHOW_ABBREVIATION_LONG{"--show-abbreviation"};
const std::string CommandLineParser::SHOW_COMMENT_LONG{"--show-comment"};
const std::string CommandLineParser::DEFAULT_IN_VALUE{"DE"};
const std::string CommandLineParser::DEFAULT_OUT_VALUE{"EN"};
const bool CommandLineParser::DEFAULT_SHOW_PHRASE{true};
const bool CommandLineParser::DEFAULT_SHOW_WORD_CLASS{false};
const bool CommandLineParser::DEFAULT_SHOW_GENDER{false};
const bool CommandLineParser::DEFAULT_SHOW_NUMERUS{false};
const bool CommandLineParser::DEFAULT_SHOW_ABBREVIATION{false};
const bool CommandLineParser::DEFAULT_SHOW_COMMENT{false};

CommandLineParser::CommandLineParser() : CommandLineParser{DEFAULT_IN_VALUE, DEFAULT_OUT_VALUE} {}

CommandLineParser::CommandLineParser(const std::string &default_in, const std::string &default_out)
    : app_name_{""},
      default_in_{default_in},
      default_out_{default_out},
      in_{""},
      out_{""},
      phrase_{""},
      language_id_{""},
      language_name_{""},
      word_class_id_{""},
      word_class_name_{""},
      gender_id_{""},
      gender_name_{""},
      has_phrase_{},
      help_{},
      all_languages_{},
      all_word_classes_{},
      all_genders_{},
      all_numeri_{},
      has_language_id_{},
      has_language_name_{},
      has_word_class_id_{},
      has_word_class_name_{},
      has_gender_id_{},
      has_gender_name_{},
      show_phrase_{DEFAULT_SHOW_PHRASE},
      has_show_phrase_{},
      show_word_class_{DEFAULT_SHOW_WORD_CLASS},
      has_show_word_class_{},
      show_gender_{DEFAULT_SHOW_GENDER},
      has_show_gender_{},
      show_numerus_{DEFAULT_SHOW_NUMERUS},
      has_show_numerus_{},
      show_abbreviation_{DEFAULT_SHOW_ABBREVIATION},
      has_show_abbreviation_{},
      show_comment_{DEFAULT_SHOW_COMMENT},
      has_show_comment_{} {}

CommandLineParser &CommandLineParser::operator()(const int argc, const char **argv) {
  bool in_found{false};
  bool out_found{false};
  if (argc < 1) throw CommandLineException{"No command line arguments found!"};
  this->app_name_ = argv[0];
  for (int i{1}; argc > i; ++i) {
    if (HELP_NAME_LONG == argv[i] || HELP_NAME_SHORT == argv[i]) {
      this->help_ = true;
    } else if ((IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->in_ = argv[++i];
      in_found = true;
    } else if (IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if ((OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->out_ = argv[++i];
      out_found = true;
    } else if (OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (ALL_LANGUAGES_LONG == argv[i]) {
      this->all_languages_ = true;
    } else if (LANGUAGE_ID_LONG == argv[i] && argc - 1 != i) {
      this->language_id_ = argv[++i];
      this->has_language_id_ = true;
    } else if (LANGUAGE_ID_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (LANGUAGE_NAME_LONG == argv[i] && argc - 1 != i) {
      this->language_name_ = argv[++i];
      this->has_language_name_ = true;
    } else if (LANGUAGE_NAME_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (ALL_WORD_CLASSES_LONG == argv[i]) {
      this->all_word_classes_ = true;
    } else if (WORD_CLASS_ID_LONG == argv[i] && argc - 1 != i) {
      this->word_class_id_ = argv[++i];
      this->has_word_class_id_ = true;
    } else if (WORD_CLASS_ID_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (WORD_CLASS_NAME_LONG == argv[i] && argc - 1 != i) {
      this->word_class_name_ = argv[++i];
      this->has_word_class_name_ = true;
    } else if (WORD_CLASS_NAME_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (ALL_GENDERS_LONG == argv[i]) {
      this->all_genders_ = true;
    } else if (GENDER_ID_LONG == argv[i] && argc - 1 != i) {
      this->gender_id_ = argv[++i];
      this->has_gender_id_ = true;
    } else if (GENDER_ID_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (GENDER_NAME_LONG == argv[i] && argc - 1 != i) {
      this->gender_name_ = argv[++i];
      this->has_gender_name_ = true;
    } else if (GENDER_NAME_LONG == argv[i]) {
      throw CommandLineException{"The value for \"" + std::string{argv[i]} + "\" is missing!"};
    } else if (ALL_NUMERI_LONG == argv[i]) {
      this->all_numeri_ = true;
    } else if (argc - 1 == i) {
      this->phrase_ = argv[i];
      this->has_phrase_ = true;
    } else if (SHOW_PHRASE_LONG == argv[i]) {
      this->show_phrase_ = true;
      this->has_show_phrase_ = true;
    } else if (SHOW_WORD_CLASS_LONG == argv[i]) {
      this->show_word_class_ = true;
      this->has_show_word_class_ = true;
    } else if (SHOW_GENDER_LONG == argv[i]) {
      this->show_gender_ = true;
      this->has_show_gender_ = true;
    } else if (SHOW_NUMERUS_LONG == argv[i]) {
      this->show_numerus_ = true;
      this->has_show_numerus_ = true;
    } else if (SHOW_ABBREVIATION_LONG == argv[i]) {
      this->show_abbreviation_ = true;
      this->has_show_abbreviation_ = true;
    } else if (SHOW_COMMENT_LONG == argv[i]) {
      this->show_comment_ = true;
      this->has_show_comment_ = true;
    } else {
      throw CommandLineException{"The argument \"" + std::string{argv[i]} + "\" is not known!"};
    }
  }  // for (int i{1}; argc > i; ++i)

  // check exclusive arguments (if --help|-h is not set)
  short exclusive_arguments{0};
  if (this->has_language_id_) ++exclusive_arguments;
  if (this->has_language_name_) ++exclusive_arguments;
  if (this->has_word_class_id_) ++exclusive_arguments;
  if (this->has_word_class_name_) ++exclusive_arguments;
  if (this->has_gender_id_) ++exclusive_arguments;
  if (this->has_gender_name_) ++exclusive_arguments;
  if (exclusive_arguments > 1) {
    throw CommandLineException{"The arguments \"" + LANGUAGE_ID_LONG + "\", \"" + LANGUAGE_NAME_LONG + "\", \"" +
                               WORD_CLASS_ID_LONG + "\", \"" + WORD_CLASS_NAME_LONG + "\", \"" + GENDER_ID_LONG +
                               "\" and \"" + GENDER_NAME_LONG +
                               "\" must be given exclusively, but found more than one occurrence!"};
  }

  if (!in_found) this->in_ = this->default_in_;
  if (!out_found) this->out_ = this->default_out_;

  return *this;
}  // operator()

std::string CommandLineParser::in() const noexcept { return this->in_; }

std::string CommandLineParser::out() const noexcept { return this->out_; }

std::string CommandLineParser::phrase() const noexcept { return this->phrase_; }

bool CommandLineParser::has_phrase() const noexcept { return this->has_phrase_; }

bool CommandLineParser::help() const noexcept { return this->help_; }

std::string CommandLineParser::usage() const noexcept {
  return "Usage of " + this->app_name_ + " [options] phrase\n" +
         "Command line frontend for a RESTful translation service\n\n" + "Arguments:\n" + IN_NAME_SHORT + "|" +
         IN_NAME_LONG + " <language ID>               language identifier for the source phrase,\n" +
         "                                    default: \"" + this->default_in_ + "\"\n" + OUT_NAME_SHORT + "|" +
         OUT_NAME_LONG + " <language ID>              language identifier for the target phrase,\n" +
         "                                    default: \"" + this->default_out_ + "\"\n" + HELP_NAME_SHORT + "|" +
         HELP_NAME_LONG + "                           show this message\n" + ALL_LANGUAGES_LONG +
         "                     show all languages that are available\n" + LANGUAGE_ID_LONG +
         " <language ID>         show the language name for the given\n" +
         "                                    language ID\n" + LANGUAGE_NAME_LONG +
         " <language name>     show the language ID for the given language\n" +
         "                                    name\n" + ALL_WORD_CLASSES_LONG +
         "                  show all word classes that are available\n" + WORD_CLASS_ID_LONG +
         " <word class ID>     show the word class name for the given word\n" +
         "                                    class ID\n" + WORD_CLASS_NAME_LONG +
         " <word class name> show the word class ID for the given word\n" +
         "                                    class name\n" + ALL_GENDERS_LONG +
         "                       show all genders that are available\n" + GENDER_ID_LONG +
         " <gender ID>             show the gender name for the given gender\n" +
         "                                    ID\n" + GENDER_NAME_LONG +
         " <gender name>         show the gender ID for the given gender\n" +
         "                                    name\n" + ALL_NUMERI_LONG +
         "                        show all numeri that are available\n" + SHOW_PHRASE_LONG +
         "                       show the phrase part of a translation\n" +
         "                                    result (default)\n" + SHOW_WORD_CLASS_LONG +
         "                   show the word class part of a translation\n" +
         "                                    result\n" + SHOW_GENDER_LONG +
         "                       show the gender part of a translation\n" +
         "                                    result\n" + SHOW_NUMERUS_LONG +
         "                      show the numerus part of a translation\n" +
         "                                    result\n" + SHOW_ABBREVIATION_LONG +
         "                 show the abbreviation part of a translation\n" +
         "                                    result\n" + SHOW_COMMENT_LONG +
         "                      show the comment part of a translation\n" +
         "                                    result\n";
}

bool CommandLineParser::all_languages() const noexcept { return this->all_languages_; }

bool CommandLineParser::all_word_classes() const noexcept { return this->all_word_classes_; }

bool CommandLineParser::all_genders() const noexcept { return this->all_genders_; }

bool CommandLineParser::all_numeri() const noexcept { return this->all_numeri_; }

bool CommandLineParser::has_language_id() const noexcept { return this->has_language_id_; }

std::string CommandLineParser::language_id() const noexcept { return this->language_id_; }

bool CommandLineParser::has_language_name() const noexcept { return this->has_language_name_; }

std::string CommandLineParser::language_name() const noexcept { return this->language_name_; }

bool CommandLineParser::has_word_class_id() const noexcept { return this->has_word_class_id_; }

std::string CommandLineParser::word_class_id() const noexcept { return this->word_class_id_; }

bool CommandLineParser::has_word_class_name() const noexcept { return this->has_word_class_name_; }

std::string CommandLineParser::word_class_name() const noexcept { return this->word_class_name_; }

bool CommandLineParser::has_gender_id() const noexcept { return this->has_gender_id_; }

std::string CommandLineParser::gender_id() const noexcept { return this->gender_id_; }

bool CommandLineParser::has_gender_name() const noexcept { return this->has_gender_name_; }

std::string CommandLineParser::gender_name() const noexcept { return this->gender_name_; }

bool CommandLineParser::show_phrase() const noexcept { return this->show_phrase_; }

bool CommandLineParser::has_show_phrase() const noexcept { return this->has_show_phrase_; }

bool CommandLineParser::show_word_class() const noexcept { return this->show_word_class_; }

bool CommandLineParser::has_show_word_class() const noexcept { return this->has_show_word_class_; }

bool CommandLineParser::show_gender() const noexcept { return this->show_gender_; }

bool CommandLineParser::has_show_gender() const noexcept { return this->has_show_gender_; }

bool CommandLineParser::show_numerus() const noexcept { return this->show_numerus_; }

bool CommandLineParser::has_show_numerus() const noexcept { return this->has_show_numerus_; }

bool CommandLineParser::show_abbreviation() const noexcept { return this->show_abbreviation_; }

bool CommandLineParser::has_show_abbreviation() const noexcept { return this->has_show_abbreviation_; }

bool CommandLineParser::show_comment() const noexcept { return this->show_comment_; }

bool CommandLineParser::has_show_comment() const noexcept { return this->has_show_comment_; }

}  // translate
}  // lgeorgieff
