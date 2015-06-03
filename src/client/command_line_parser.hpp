// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/29/2015
// Description: Declares the interface of the command line parser which is adapted to the supported options of this
//              program.
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

#ifndef COMMAND_LINE_PARSER_HPP_
#define COMMAND_LINE_PARSER_HPP_

#include <string>

namespace lgeorgieff {
namespace translate {
class CommandLineParser {
 public:
  // The name of the option representing the language of the incomming text (origin)
  static const std::string IN_NAME_LONG;
  static const std::string IN_NAME_SHORT;
  // The name of the option representing the language of the outcomming text (result)
  static const std::string OUT_NAME_LONG;
  static const std::string OUT_NAME_SHORT;
  // The name of the option representing the help switch
  static const std::string HELP_NAME_LONG;
  static const std::string HELP_NAME_SHORT;
  // The name of the option for getting all languages
  static const std::string ALL_LANGUAGES_LONG;
  // The name of the option for getting the language name of the passed language ID
  static const std::string LANGUAGE_ID_LONG;
  // The name of the option for getting the language ID of the passed language name
  static const std::string LANGUAGE_NAME_LONG;
  // The name of the option for getting all word classes
  static const std::string ALL_WORD_CLASSES_LONG;
  // The name of the option for getting the word class name of the passed word class ID
  static const std::string WORD_CLASS_ID_LONG;
  // The name of the option for getting the word class ID of the passed word class name
  static const std::string WORD_CLASS_NAME_LONG;
  // The name of the option for getting all genders
  static const std::string ALL_GENDERS_LONG;
  // The name of the option for getting the gender name of the passed gender ID
  static const std::string GENDER_ID_LONG;
  // The name of the option for getting the gender ID of the passed gender name
  static const std::string GENDER_NAME_LONG;
  // The name of the option for getting all numeri
  static const std::string ALL_NUMERI_LONG;
  // The name of the option for showing the phrase of a final result
  static const std::string SHOW_PHRASE_LONG;
  // The name of the option for showing the word class of a final result
  static const std::string SHOW_WORD_CLASS_LONG;
  // The name of the option for showing the gender of a final result
  static const std::string SHOW_GENDER_LONG;
  // The name of the option for showing the numerus of a final result
  static const std::string SHOW_NUMERUS_LONG;
  // The name of the option for showing the abbreviation of a final result
  static const std::string SHOW_ABBREVIATION_LONG;
  // The name of the option for showing the comment of a final result
  static const std::string SHOW_COMMENT_LONG;
  // The default language value of the incomming text (origin)
  static const std::string DEFAULT_IN_VALUE;
  // The default language value of the outcomming text (result)
  static const std::string DEFAULT_OUT_VALUE;
  // The default value whether the phrase of an result is printed or not
  static const bool DEFAULT_SHOW_PHRASE;
  // The default value whether the word class of an result is printed or not
  static const bool DEFAULT_SHOW_WORD_CLASS;
  // The default value whether the gender of an result is printed or not
  static const bool DEFAULT_SHOW_GENDER;
  // The default value whether the numerus of an result is printed or not
  static const bool DEFAULT_SHOW_NUMERUS;
  // The default value whether the abbreviation of an result is printed or not
  static const bool DEFAULT_SHOW_ABBREVIATION;
  // The default value whether the comment of an result is printed or not
  static const bool DEFAULT_SHOW_COMMENT;

  // The constructor that takes the command line arguments from the main function and finally initializes all
  // internal values
  CommandLineParser(const int, const char**);
  // The constructor that takes the command line arguments from the main function and particular default
  // values for DEFAULT_IN_VALUE and DEFAULT_OUT_VALUE and finally initializes all internal values
  CommandLineParser(const int, const char**, const std::string&, const std::string&);
  CommandLineParser(const CommandLineParser&) = default;
  CommandLineParser(CommandLineParser&&) = default;

  CommandLineParser& operator=(const CommandLineParser&) = default;
  CommandLineParser& operator=(CommandLineParser&&) = default;

  // The destructor
  ~CommandLineParser() = default;

  // A getter for the laguage of the incoming text (origin)
  std::string in() const noexcept;
  // A getter for the laguage of the outcoming text (result)
  std::string out() const noexcept;
  // A getter for the text to be translated
  std::string phrase() const noexcept;
  // A getter that indicates whether a phrase was set or not
  bool has_phrase() const noexcept;
  // A getter that indicates whether the help switch is given or not
  bool help() const noexcept;
  // A getter that indicates whether all languages are requested or not
  bool all_languages() const noexcept;
  // A getter that indicates whether all word classes are requested or not
  bool all_word_classes() const noexcept;
  // A getter that indicates whether all genders are requested or not
  bool all_genders() const noexcept;
  // A getter that indicates whether all numeri are requested or not
  bool all_numeri() const noexcept;
  // A getter that indicates whether a language id was passed or not
  bool has_language_id() const noexcept;
  // A getter for the language id
  std::string language_id() const noexcept;
  // A getter that indicates whether a language name was passed or not
  bool has_language_name() const noexcept;
  // A getter for the language name
  std::string language_name() const noexcept;
  // A getter that indicates whether a word class id was passed or not
  bool has_word_class_id() const noexcept;
  // A getter for the word class id
  std::string word_class_id() const noexcept;
  // A getter that indicates whether a word class name was passed or not
  bool has_word_class_name() const noexcept;
  // A getter for the word class name
  std::string word_class_name() const noexcept;
  // A getter that indicates whether a gender id was passed or not
  bool has_gender_id() const noexcept;
  // A getter for the gender id
  std::string gender_id() const noexcept;
  // A getter that indicates whether a gender name was passed or not
  bool has_gender_name() const noexcept;
  // A getter for the gender name
  std::string gender_name() const noexcept;
  // A getter indicating whether to show the phrase from the result or not
  bool show_phrase() const noexcept;
  // A getter indicating whether to show the word class from the result or not
  bool show_word_class() const noexcept;
  // A getter indicating whether to show the gender from the result or not
  bool show_gender() const noexcept;
  // A getter indicating whether to show the numerus from the result or not
  bool show_numerus() const noexcept;
  // A getter indicating whether to show the abbreviation from the result or not
  bool show_abbreviation() const noexcept;
  // A getter indicating whether to show the comment from the result or not
  bool show_comment() const noexcept;
  
  // Returns a string representing the usage corresponding to the CLI options
  std::string usage() const noexcept;

 private:
  std::string app_name_;
  std::string usage_;
  std::string in_;
  std::string out_;
  std::string phrase_;
  std::string language_id_;
  std::string language_name_;
  std::string word_class_id_;
  std::string word_class_name_;
  std::string gender_id_;
  std::string gender_name_;
  bool has_phrase_;
  bool help_;
  bool all_languages_;
  bool all_word_classes_;
  bool all_genders_;
  bool all_numeri_;
  bool has_language_id_;
  bool has_language_name_;
  bool has_word_class_id_;
  bool has_word_class_name_;
  bool has_gender_id_;
  bool has_gender_name_;
  bool show_phrase_;
  bool show_word_class_;
  bool show_gender_;
  bool show_numerus_;
  bool show_abbreviation_;
  bool show_comment_;
};  // CommandLineParser
}  // translate
}  // lgeorgieff

#endif  // COMMAND_LINE_PARSER_HPP_
