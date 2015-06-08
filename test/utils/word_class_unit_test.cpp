// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/05/2015
// Description: Unit test for the WordClass enum class and its helper functions.
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

#include "utils/word_class.hpp"
#include "utils/exception.hpp"

#include <string>

using std::string;

using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::WordClass;
using lgeorgieff::translate::utils::to_string;
using lgeorgieff::translate::utils::to_db_string;
using lgeorgieff::translate::utils::from_string;
using lgeorgieff::translate::utils::from_db_string;
using lgeorgieff::translate::utils::is_word_class;

TEST(word_class, to_string) {
  EXPECT_EQ(string{"adj"}, to_string(WordClass::adj));
  EXPECT_EQ(string{"adv"}, to_string(WordClass::adv));
  EXPECT_EQ(string{"past-p"}, to_string(WordClass::pastp));
  EXPECT_EQ(string{"verb"}, to_string(WordClass::verb));
  EXPECT_EQ(string{"pres-p"}, to_string(WordClass::presp));
  EXPECT_EQ(string{"prep"}, to_string(WordClass::prep));
  EXPECT_EQ(string{"conj"}, to_string(WordClass::conj));
  EXPECT_EQ(string{"pron"}, to_string(WordClass::pron));
  EXPECT_EQ(string{"prefix"}, to_string(WordClass::prefix));
  EXPECT_EQ(string{"suffix"}, to_string(WordClass::suffix));
  EXPECT_EQ(string{"noun"}, to_string(WordClass::noun));
  EXPECT_EQ(string{"art"}, to_string(WordClass::art));
  EXPECT_EQ(string{"num"}, to_string(WordClass::num));
  EXPECT_EQ(string{"interj"}, to_string(WordClass::interj));
  EXPECT_EQ(string{"phrase"}, to_string(WordClass::phrase));
  EXPECT_EQ(string{"idiom"}, to_string(WordClass::idiom));
  EXPECT_EQ(string{""}, to_string(WordClass::none));
}

TEST(word_class, to_db_string) {
  EXPECT_EQ(string{"'adj'"}, to_db_string(WordClass::adj));
  EXPECT_EQ(string{"'adv'"}, to_db_string(WordClass::adv));
  EXPECT_EQ(string{"'past-p'"}, to_db_string(WordClass::pastp));
  EXPECT_EQ(string{"'verb'"}, to_db_string(WordClass::verb));
  EXPECT_EQ(string{"'pres-p'"}, to_db_string(WordClass::presp));
  EXPECT_EQ(string{"'prep'"}, to_db_string(WordClass::prep));
  EXPECT_EQ(string{"'conj'"}, to_db_string(WordClass::conj));
  EXPECT_EQ(string{"'pron'"}, to_db_string(WordClass::pron));
  EXPECT_EQ(string{"'prefix'"}, to_db_string(WordClass::prefix));
  EXPECT_EQ(string{"'suffix'"}, to_db_string(WordClass::suffix));
  EXPECT_EQ(string{"'noun'"}, to_db_string(WordClass::noun));
  EXPECT_EQ(string{"'art'"}, to_db_string(WordClass::art));
  EXPECT_EQ(string{"'num'"}, to_db_string(WordClass::num));
  EXPECT_EQ(string{"'interj'"}, to_db_string(WordClass::interj));
  EXPECT_EQ(string{"'phrase'"}, to_db_string(WordClass::phrase));
  EXPECT_EQ(string{"'idiom'"}, to_db_string(WordClass::idiom));
  EXPECT_EQ(string{"null"}, to_db_string(WordClass::none));
}

TEST(word_class, from_string) {
  EXPECT_EQ(WordClass::adj, from_string<WordClass>("adj"));
  EXPECT_EQ(WordClass::adv, from_string<WordClass>("adv"));
  EXPECT_EQ(WordClass::pastp, from_string<WordClass>("past-p"));
  EXPECT_EQ(WordClass::verb, from_string<WordClass>("verb"));
  EXPECT_EQ(WordClass::presp, from_string<WordClass>("pres-p"));
  EXPECT_EQ(WordClass::prep, from_string<WordClass>("prep"));
  EXPECT_EQ(WordClass::conj, from_string<WordClass>("conj"));
  EXPECT_EQ(WordClass::pron, from_string<WordClass>("pron"));
  EXPECT_EQ(WordClass::prefix, from_string<WordClass>("prefix"));
  EXPECT_EQ(WordClass::suffix, from_string<WordClass>("suffix"));
  EXPECT_EQ(WordClass::noun, from_string<WordClass>("noun"));
  EXPECT_EQ(WordClass::art, from_string<WordClass>("art"));
  EXPECT_EQ(WordClass::num, from_string<WordClass>("num"));
  EXPECT_EQ(WordClass::interj, from_string<WordClass>("interj"));
  EXPECT_EQ(WordClass::phrase, from_string<WordClass>("phrase"));
  EXPECT_EQ(WordClass::idiom, from_string<WordClass>("idiom"));
  EXPECT_EQ(WordClass::none, from_string<WordClass>(""));
  EXPECT_THROW(from_string<WordClass>("''"), Exception);
  EXPECT_THROW(from_string<WordClass>("'pron'"), Exception);
  EXPECT_THROW(from_string<WordClass>("null"), Exception);
  EXPECT_THROW(from_string<WordClass>("none"), Exception);
  EXPECT_THROW(from_string<WordClass>("Art"), Exception);
  EXPECT_THROW(from_string<WordClass>("'phrase'"), Exception);
}

TEST(word_class, from_db_string) {
  EXPECT_EQ(WordClass::adj, from_db_string<WordClass>("'adj'"));
  EXPECT_EQ(WordClass::adv, from_db_string<WordClass>("'adv'"));
  EXPECT_EQ(WordClass::pastp, from_db_string<WordClass>("'past-p'"));
  EXPECT_EQ(WordClass::verb, from_db_string<WordClass>("'verb'"));
  EXPECT_EQ(WordClass::presp, from_db_string<WordClass>("'pres-p'"));
  EXPECT_EQ(WordClass::prep, from_db_string<WordClass>("'prep'"));
  EXPECT_EQ(WordClass::conj, from_db_string<WordClass>("'conj'"));
  EXPECT_EQ(WordClass::pron, from_db_string<WordClass>("'pron'"));
  EXPECT_EQ(WordClass::prefix, from_db_string<WordClass>("'prefix'"));
  EXPECT_EQ(WordClass::suffix, from_db_string<WordClass>("'suffix'"));
  EXPECT_EQ(WordClass::noun, from_db_string<WordClass>("'noun'"));
  EXPECT_EQ(WordClass::art, from_db_string<WordClass>("'art'"));
  EXPECT_EQ(WordClass::num, from_db_string<WordClass>("'num'"));
  EXPECT_EQ(WordClass::interj, from_db_string<WordClass>("'interj'"));
  EXPECT_EQ(WordClass::phrase, from_db_string<WordClass>("'phrase'"));
  EXPECT_EQ(WordClass::idiom, from_db_string<WordClass>("'idiom'"));
  EXPECT_EQ(WordClass::none, from_db_string<WordClass>("null"));
  EXPECT_THROW(from_db_string<WordClass>("''"), Exception);
  EXPECT_THROW(from_db_string<WordClass>("pron"), Exception);
  EXPECT_THROW(from_db_string<WordClass>("'null'"), Exception);
  EXPECT_THROW(from_db_string<WordClass>("none"), Exception);
  EXPECT_THROW(from_db_string<WordClass>("'Art'"), Exception);
}

TEST(word_class, is_word_class) {
  EXPECT_TRUE(is_word_class(string{"adj"}));
  EXPECT_TRUE(is_word_class(string{"adv"}));
  EXPECT_TRUE(is_word_class(string{"past-p"}));
  EXPECT_TRUE(is_word_class(string{"verb"}));
  EXPECT_TRUE(is_word_class(string{"pres-p"}));
  EXPECT_TRUE(is_word_class(string{"prep"}));
  EXPECT_TRUE(is_word_class(string{"conj"}));
  EXPECT_TRUE(is_word_class(string{"pron"}));
  EXPECT_TRUE(is_word_class(string{"prefix"}));
  EXPECT_TRUE(is_word_class(string{"suffix"}));
  EXPECT_TRUE(is_word_class(string{"noun"}));
  EXPECT_TRUE(is_word_class(string{"art"}));
  EXPECT_TRUE(is_word_class(string{"num"}));
  EXPECT_TRUE(is_word_class(string{"interj"}));
  EXPECT_TRUE(is_word_class(string{"phrase"}));
  EXPECT_TRUE(is_word_class(string{"idiom"}));
  EXPECT_TRUE(is_word_class(string{""}));
  EXPECT_FALSE(is_word_class(string{"Idiom"}));
  EXPECT_FALSE(is_word_class(string{"''"}));
  EXPECT_FALSE(is_word_class(string{"'idiom'"}));
  EXPECT_FALSE(is_word_class(string{"none"}));
  EXPECT_FALSE(is_word_class(string{"null"}));
}
