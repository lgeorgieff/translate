// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/13/2015
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

#include <string>
#include <cstring>
#include <utility>

using lgeorgieff::translate::utils::CommandLineException;
using std::string;

namespace lgeorgieff {
namespace translate {

string CommandLineParser::IN_NAME_LONG{"--in"};
string CommandLineParser::IN_NAME_SHORT{"-i"};
string CommandLineParser::OUT_NAME_LONG{"--out"};
string CommandLineParser::OUT_NAME_SHORT{"-o"};
string CommandLineParser::HELP_NAME_LONG{"--help"};
string CommandLineParser::HELP_NAME_SHORT{"-h"};
string CommandLineParser::DEFAULT_IN_VALUE{"de"};
string CommandLineParser::DEFAULT_OUT_VALUE{"en"};

CommandLineParser::CommandLineParser(const int argc, const char **argv)
    : CommandLineParser{argc, argv, DEFAULT_IN_VALUE, DEFAULT_OUT_VALUE} {}

CommandLineParser::CommandLineParser(const int argc, const char **argv, const string &default_in,
                                     const string &default_out)
    : in_{""}, out_{""}, phrase_{""}, help_{false} {
  bool in_found{false};
  bool out_found{false};
  for (int i{1}; argc > i; ++i) {
    if(HELP_NAME_LONG == argv[i] || HELP_NAME_SHORT == argv[i]) {
      this->help_ = true;
    } else if ((IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->in_ = string(argv[++i]);
      in_found = true;
    } else if (IN_NAME_LONG == argv[i] || IN_NAME_SHORT == argv[i]) {
      throw CommandLineException("The value for \"" + string(argv[i]) + "\" is missing!");
    } else if ((OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) && argc - 1 != i) {
      this->out_ = string(argv[++i]);
      out_found = true;
    } else if (OUT_NAME_LONG == argv[i] || OUT_NAME_SHORT == argv[i]) {
      throw CommandLineException("The value for \"" + string(argv[i]) + "\" is missing!");
    } else if (argc - 1 == i) {
      phrase_ = string(argv[i]);
    } else {
      throw CommandLineException("The argument \"" + string(argv[i]) + "\" is not known!");
    }
  }

  if (!in_found) this->in_ = default_in;
  if (!out_found) this->out_ = default_out;
}

string CommandLineParser::in() const noexcept { return this->in_; }

string CommandLineParser::out() const noexcept { return this->out_; }

string CommandLineParser::phrase() const noexcept { return this->phrase_; }

bool CommandLineParser::help() const noexcept { return this->help_; }

}  // translate
}  // lgeorgieff
