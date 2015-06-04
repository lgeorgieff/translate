// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/04/2015
// Description: Unit test for the command_line_parser class
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

#include "gtest/gtest.h"

#include "../../client/command_line_parser.hpp"
#include "../../utils/command_line_exception.hpp"

#include <cstddef>
#include <string>

using lgeorgieff::translate::CommandLineParser;
using lgeorgieff::translate::utils::CommandLineException;

using std::string;

TEST(command_line_parser, constants) {
  EXPECT_EQ(string{"-h"}, CommandLineParser::HELP_NAME_SHORT);
  EXPECT_EQ(string{"--help"}, CommandLineParser::HELP_NAME_LONG);
  EXPECT_EQ(string{"-i"}, CommandLineParser::IN_NAME_SHORT);
  EXPECT_EQ(string{"--in"}, CommandLineParser::IN_NAME_LONG);
  EXPECT_EQ(string{"-o"}, CommandLineParser::OUT_NAME_SHORT);
  EXPECT_EQ(string{"--out"}, CommandLineParser::OUT_NAME_LONG);
  EXPECT_EQ(string{"DE"}, CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(string{"EN"}, CommandLineParser::DEFAULT_OUT_VALUE);
}

TEST(command_line_parser, short_arguments) {
  CommandLineParser clp{};
  clp(2, (const char* []){"app", "-h"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "-i", "in"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "-o", "out"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "-i", "in", "-o", "out"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "-h", "-i", "in"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "-h", "-i", "in", "-o", "out"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "-i", "in1", "-i", "in2"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in2"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "-o", "out1", "-o", "out2", "phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), string{"out2"});
  EXPECT_EQ(clp.phrase(), string{"phrase"});
  
  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "-h", "phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "-i", "in", "another phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "-o", "out", "phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{"phrase"});

  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "-i", "in", "-o", "out", "another phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "-h", "-i", "in", "another phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(8, (const char* []){"app", "-i", "in", "-h", "-o", "out", "-h", "another phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(2, (const char* []){"app", "-j"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"-j"});

  clp = CommandLineParser{};
  clp(2, (const char* []){"app", "only a phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  EXPECT_THROW(clp(3, (const char* []){"app", "-j", "abc"}), CommandLineException);  
  EXPECT_THROW(clp(5, (const char* []){"app", "-i", "in", "-h", "-o"}), CommandLineException);
  EXPECT_THROW(clp(4, (const char* []){"app", "-o", "out", "-i"}), CommandLineException);
  EXPECT_THROW(clp(7, (const char* []){"app", "-i", "in", "-o", "out", "another phrase", "-h"}), CommandLineException);
  EXPECT_THROW(clp(7, (const char* []){"app", "-i", "in1", "-i", "in2", "phrase1", "phrase2"}), CommandLineException);
} // TEST(command_line_parser, short_arguments)

TEST(command_line_parser, long_arguments) {
  CommandLineParser clp{};
  clp(2, (const char* []){"app", "--help"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--in", "in"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--out", "out"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "--in", "in", "--out", "out"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--help", "--in", "in"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "--help", "--in", "in", "--out", "out"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "--in", "in1", "--in", "in2"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in2"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{""});
  
  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "--out", "out1", "--out", "out2", "phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), string{"out2"});
  EXPECT_EQ(clp.phrase(), string{"phrase"});
  
  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--help", "phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--in", "in", "another phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--out", "out", "phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string{"phrase"});

  clp = CommandLineParser{};
  clp(6, (const char* []){"app", "--in", "in", "--out", "out", "another phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(5, (const char* []){"app", "--help", "--in", "in", "another phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(8, (const char* []){"app", "--in", "in", "--help", "--out", "out", "--help", "another phrase"});
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), string{"in"});
  EXPECT_EQ(clp.out(), string{"out"});
  EXPECT_EQ(clp.phrase(), string{"another phrase"});

  clp = CommandLineParser{};
  clp(2, (const char* []){"app", "--jj"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"--jj"});

  clp = CommandLineParser{};
  clp(2, (const char* []){"app", "only a phrase"});
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  EXPECT_THROW(clp(3, (const char* []){"app", "--jj", "abc"}), CommandLineException);
  EXPECT_THROW(clp(5, (const char* []){"app", "--in", "in", "--help", "--out"}), CommandLineException);
  EXPECT_THROW(clp(4, (const char* []){"app", "--out", "out", "--in"}), CommandLineException);
  EXPECT_THROW(clp(7, (const char* []){"app", "--in", "in", "--out", "out", "another phrase", "--help"}),
               CommandLineException);
  EXPECT_THROW(clp(7, (const char* []){"app", "--in", "in1", "--in", "in2", "phrase1", "phrase2"}),
               CommandLineException);
} // TEST(command_line_parser, long_arguments)

TEST(command_line_parser, language_arguments) {
  CommandLineParser clp = CommandLineParser{};
  clp(2, (const char* []){"app", "--all-languages"});
  EXPECT_TRUE(clp.all_languages());
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--language-id", "PL"});
  EXPECT_TRUE(clp.has_language_id());
  EXPECT_EQ(clp.language_id(), string{"PL"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--language-name", "polski"});
  EXPECT_TRUE(clp.has_language_name());
  EXPECT_EQ(clp.language_name(), string{"polski"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--all-languages", "only a phrase"});
  EXPECT_TRUE(clp.all_languages());
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--language-id", "PL", "only a phrase"});
  EXPECT_TRUE(clp.has_language_id());
  EXPECT_EQ(clp.language_id(), string{"PL"});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--language-name", "polski", "only a phrase"});
  EXPECT_TRUE(clp.has_language_name());
  EXPECT_EQ(clp.language_name(), string{"polski"});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  EXPECT_THROW(clp(2, (const char* []){"app", "--language-id"}), CommandLineException);
  EXPECT_THROW(clp(2, (const char* []){"app", "--language-name"}), CommandLineException);
}

TEST(command_line_parser, word_class_arguments) {
  CommandLineParser clp = CommandLineParser{};
  clp(2, (const char* []){"app", "--all-word-classes"});
  EXPECT_TRUE(clp.all_word_classes());
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--word-class-id", "noun"});
  EXPECT_TRUE(clp.has_word_class_id());
  EXPECT_EQ(clp.word_class_id(), string{"noun"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--word-class-name", "noun"});
  EXPECT_TRUE(clp.has_word_class_name());
  EXPECT_EQ(clp.word_class_name(), string{"noun"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--all-word-classes", "only a phrase"});
  EXPECT_TRUE(clp.all_word_classes());
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--word-class-id", "noun", "only a phrase"});
  EXPECT_TRUE(clp.has_word_class_id());
  EXPECT_EQ(clp.word_class_id(), string{"noun"});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--word-class-name", "noun", "only a phrase"});
  EXPECT_TRUE(clp.has_word_class_name());
  EXPECT_EQ(clp.word_class_name(), string{"noun"});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  EXPECT_THROW(clp(2, (const char* []){"app", "--word-class-id"}), CommandLineException);
  EXPECT_THROW(clp(2, (const char* []){"app", "--word-class-name"}), CommandLineException);
}

TEST(command_line_parser, gender_arguments) {
  CommandLineParser clp = CommandLineParser{};
  clp(2, (const char* []){"app", "--all-genders"});
  EXPECT_TRUE(clp.all_genders());
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--gender-id", "f."});
  EXPECT_TRUE(clp.has_gender_id());
  EXPECT_EQ(clp.gender_id(), string{"f."});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--gender-name", "female"});
  EXPECT_TRUE(clp.has_gender_name());
  EXPECT_EQ(clp.gender_name(), string{"female"});
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--all-genders", "only a phrase"});
  EXPECT_TRUE(clp.all_genders());
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--gender-id", "m.", "only a phrase"});
  EXPECT_TRUE(clp.has_gender_id());
  EXPECT_EQ(clp.gender_id(), string{"m."});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  clp(4, (const char* []){"app", "--gender-name", "male", "only a phrase"});
  EXPECT_TRUE(clp.has_gender_name());
  EXPECT_EQ(clp.gender_name(), string{"male"});
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});

  clp = CommandLineParser{};
  EXPECT_THROW(clp(2, (const char* []){"app", "--gender-id"}), CommandLineException);
  EXPECT_THROW(clp(2, (const char* []){"app", "--gender-name"}), CommandLineException);
}

TEST(command_line_parser, numeri_arguments) {
  CommandLineParser clp = CommandLineParser{};
  clp(2, (const char* []){"app", "--all-numeri"});
  EXPECT_TRUE(clp.all_numeri());
  EXPECT_EQ(clp.phrase(), string{""});

  clp = CommandLineParser{};
  clp(3, (const char* []){"app", "--all-numeri", "only a phrase"});
  EXPECT_TRUE(clp.all_numeri());
  EXPECT_EQ(clp.phrase(), string{"only a phrase"});
}
