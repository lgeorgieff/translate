// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/27/2015
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
const std::string CommandLineParser::DEFAULT_IN_VALUE{"DE"};
const std::string CommandLineParser::DEFAULT_OUT_VALUE{"EN"};

CommandLineParser::CommandLineParser(const int argc, const char **argv)
    : CommandLineParser{argc, argv, DEFAULT_IN_VALUE, DEFAULT_OUT_VALUE} {}

CommandLineParser::CommandLineParser(const int argc, const char **argv, const std::string &default_in,
                                     const std::string &default_out)
    : app_name_{""},
      usage_{""},
      in_{""},
      out_{""},
      phrase_{""},
      language_id_{""},
      language_name_{""},
      word_class_id_{""},
      word_class_name_{""},
      gender_id_{""},
      gender_name_{false},
      has_phrase_{false},
      help_{false},
      all_languages_{false},
      all_word_classes_{false},
      all_genders_{false},
      all_numeri_{false},
      has_language_id_{false},
      has_language_name_{false},
      has_word_class_id_{false},
      has_word_class_name_{false},
      has_gender_id_{false},
      has_gender_name_{false} {
  bool in_found{false};
  bool out_found{false};
  if (argc < 1) throw CommandLineException("No command line arguments found!");
  this->app_name_ = argv[0];
  for (int i{1}; argc > i; ++i) {
    if (HELP_NAME_LONG == argv[i] || HELP_NAME_SHORT == argv[i]) {
      this->help_ = true;
    } else if ((IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->in_ = argv[++i];
      in_found = true;
    } else if (IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if ((OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->out_ = argv[++i];
      out_found = true;
    } else if (OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (ALL_LANGUAGES_LONG == argv[i]) {
      this->all_languages_ = true;
    } else if (LANGUAGE_ID_LONG == argv[i] && argc - 1 != i) {
      this->language_id_ = argv[++i];
      this->has_language_id_ = true;
    } else if (LANGUAGE_ID_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (LANGUAGE_NAME_LONG == argv[i] && argc - 1 != i) {
      this->language_name_ = argv[++i];
      this->has_language_name_ = true;
    } else if (LANGUAGE_NAME_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (ALL_WORD_CLASSES_LONG == argv[i]) {
      this->all_word_classes_ = true;
    } else if (WORD_CLASS_ID_LONG == argv[i] && argc - 1 != i) {
      this->word_class_id_ = argv[++i];
      this->has_word_class_id_ = true;
    } else if (WORD_CLASS_ID_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (WORD_CLASS_NAME_LONG == argv[i] && argc - 1 != i) {
      this->word_class_name_ = argv[++i];
      this->has_word_class_name_ = true;
    } else if (WORD_CLASS_NAME_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (ALL_GENDERS_LONG == argv[i]) {
      this->all_genders_ = true;
    } else if (GENDER_ID_LONG == argv[i] && argc - 1 != i) {
      this->gender_id_ = argv[++i];
      this->has_gender_id_ = true;
    } else if (GENDER_ID_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (GENDER_NAME_LONG == argv[i] && argc - 1 != i) {
      this->gender_name_ = argv[++i];
      this->has_gender_name_ = true;
    } else if (GENDER_NAME_LONG == argv[i]) {
      throw CommandLineException("The value for \"" + std::string{argv[i]} + "\" is missing!");
    } else if (ALL_NUMERI_LONG == argv[i]) {
      this->all_numeri_ = true;
    } else if (argc - 1 == i) {
      this->phrase_ = argv[i];
      this->has_phrase_ = true;
    } else {
      throw CommandLineException("The argument \"" + std::string{argv[i]} + "\" is not known!");
    }
  }  // for (int i{1}; argc > i; ++i)

  if (!in_found) this->in_ = default_in;
  if (!out_found) this->out_ = default_out;
}  // CommandLineParser::CommandLineParser

std::string CommandLineParser::in() const noexcept { return this->in_; }

std::string CommandLineParser::out() const noexcept { return this->out_; }

std::string CommandLineParser::phrase() const noexcept { return this->phrase_; }

bool CommandLineParser::has_phrase() const noexcept { return this->has_phrase_; }

bool CommandLineParser::help() const noexcept { return this->help_; }

std::string CommandLineParser::usage() const noexcept {
  return "Usage of " + this->app_name_ + " [options] phrase\n" +
         "Command line frontend for a RESTful translation service\n\n" + "Arguments:\n" + this->IN_NAME_SHORT + "|" +
         this->IN_NAME_LONG + " <language ID>               language identifier for the source phrase,\n" +
         "                                    default: \"" + this->DEFAULT_IN_VALUE + "\"\n" + this->OUT_NAME_SHORT +
         "|" + this->OUT_NAME_LONG + " <language ID>              language identifier for the target phrase,\n" +
         "                                    default: \"" + this->DEFAULT_OUT_VALUE + "\"\n" + this->HELP_NAME_SHORT +
         "|" + this->HELP_NAME_LONG + "                           show this message\n" + this->ALL_LANGUAGES_LONG +
         "                     show all languages that are available\n" + this->LANGUAGE_ID_LONG +
         " <language ID>         show the language name for the given\n" +
         "                                    language ID\n" + this->LANGUAGE_NAME_LONG +
         " <language name>     show the language ID for the given language\n" +
         "                                    name\n" + this->ALL_WORD_CLASSES_LONG +
         "                  show all word classes that are available\n" + this->WORD_CLASS_ID_LONG +
         " <word class ID>     show the word class name for the given word\n" +
         "                                    class ID\n" + this->WORD_CLASS_NAME_LONG +
         " <word class name> show the word class ID for the given word\n" +
         "                                    class name\n" + this->ALL_GENDERS_LONG +
         "                       show all genders that are available\n" + this->GENDER_ID_LONG +
         " <gender ID>             show the gender name for the given gender\n" +
         "                                    ID\n" + this->GENDER_NAME_LONG +
         " <gender name>         show the gender ID for the given gender\n" +
         "                                    name\n" + this->ALL_NUMERI_LONG +
         "                        show all numeri that are available";
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

}  // translate
}  // lgeorgieff
