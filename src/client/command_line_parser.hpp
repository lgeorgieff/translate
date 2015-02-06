// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 01/17/2015
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
  static std::string IN_NAME;
  // The name of the option representing the language of the outcomming text (result)
  static std::string OUT_NAME;
  // The default value language of the incomming text (origin)
  static std::string DEFAULT_IN_VALUE;
  // The default value language of the outcomming text (result)
  static std::string DEFAULT_OUT_VALUE;

  // The constructor that takes the command line arguments from the main function and finally initializes all
  // internal values
  CommandLineParser(const int, const char**) noexcept;
  // The constructor that takes the command line arguments from the main function and particular default
  // values for DEFAULT_IN_VALUE and DEFAULT_OUT_VALUE and finally initializes all internal values
  CommandLineParser(const int, const char**, const std::string&, const std::string&) noexcept;
  CommandLineParser(const CommandLineParser&) noexcept = default;
  CommandLineParser(CommandLineParser&&) noexcept = default;

  CommandLineParser& operator=(const CommandLineParser&) noexcept = default;
  CommandLineParser& operator=(CommandLineParser&&) noexcept = default;

  // A getter for the laguage of the incoming text (origin)
  std::string in() const noexcept;
  // A getter for the laguage of the outcoming text (result)
  std::string out() const noexcept;
  // A getter for the text to be translated
  std::string phrase() const noexcept;

  // The destructor
  ~CommandLineParser() noexcept = default;

 private:
  std::string in_;
  std::string out_;
  std::string phrase_;
};  // CommandLineParser
}  // translate
}  // lgeorgieff

#endif  // COMMAND_LINE_PARSER_HPP_
