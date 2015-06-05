// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/06/2015
// Description: Unit test for the helper mudule.
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

#include "utils/helper.hpp"
#include "utils/exception.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;;

using lgeorgieff::translate::utils::trim_left;
using lgeorgieff::translate::utils::trim_right;
using lgeorgieff::translate::utils::trim;
using lgeorgieff::translate::utils::normalize_whitespace;
using lgeorgieff::translate::utils::split_string;
using lgeorgieff::translate::utils::parse_accept_header_item;
using lgeorgieff::translate::utils::check_accept_header;

TEST(helper, trim_left) {
  string str{};
  trim_left(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  trim_left(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim_left(str);
  EXPECT_EQ(string{"d   \n"}, str);
  str= ".  \n\td   \n";
  trim_left(str);
  EXPECT_EQ(string{".  \n\td   \n"}, str);
  str = "8";
  trim_left(str);
  EXPECT_EQ(string{"8"}, str);
}

#include <iostream>
TEST(helper, trim_right) {
  string str{};
  trim_right(str);
  EXPECT_EQ(string{}, str);
  str= "\n\t   \n";
  trim_right(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim_right(str);
  EXPECT_EQ(string{"  \n\td"}, str);
  str= "  \n\td   \n.";
  trim_right(str);
  EXPECT_EQ(string{"  \n\td   \n."}, str);
  str = "8";
  trim_right(str);
  EXPECT_EQ(string{"8"}, str);
}

TEST(helper, trim) {
  string str{};
  trim(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  trim(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim(str);
  EXPECT_EQ(string{"d"}, str);
  str= ".  \n\td   \n";
  trim(str);
  EXPECT_EQ(string{".  \n\td"}, str);
  str = "8";
  trim(str);
  EXPECT_EQ(string{"8"}, str);
}

TEST(helper, normalize_whitespace) {
  string str{};
  normalize_whitespace(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{"d"}, str);
  str= ".  \n\td   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{". d"}, str);
  str = ".  \n\td   \nabc  \n\n\n\tx ";
  normalize_whitespace(str);
  EXPECT_EQ(string{". d abc x"}, str);
  str = "8";
  normalize_whitespace(str);
  EXPECT_EQ(string{"8"}, str);
}

TEST(helper, split_string) {
  vector<string> result{split_string("", ',', true)};
  EXPECT_TRUE(result.empty());
  result = {split_string("", ',')};
  EXPECT_TRUE(result.empty());
  result = {split_string("  abc defg \t", '.', true)};
  EXPECT_EQ(1, result.size());
  EXPECT_EQ("abc defg", result[0]);
  result = {split_string("  abc defg \t", '.')};
  EXPECT_EQ(1, result.size());
  EXPECT_EQ("  abc defg \t", result[0]);
  result = {split_string(" . abc .defg .\t", '.', true)};
  EXPECT_EQ(2, result.size());
  EXPECT_EQ("abc", result[0]);
  EXPECT_EQ("defg", result[1]);
  result = {split_string(" . abc .defg .\t", '.', false)};
  EXPECT_EQ(4, result.size());
  EXPECT_EQ(string{" "}, result[0]);
  EXPECT_EQ(" abc ", result[1]);
  EXPECT_EQ("defg ", result[2]);
  EXPECT_EQ("\t", result[3]);
}

TEST(helper, parse_accept_header_item) {
  string accept_type, accept_subtype;
  parse_accept_header_item("", accept_type, accept_subtype);
  EXPECT_TRUE(accept_type.empty());
  EXPECT_TRUE(accept_subtype.empty());
  parse_accept_header_item("*/*", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("  */*  ", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("  */*;", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("  */*;abc", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("*/*;abc;def", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("  */*  ; abc ;def;ghi", accept_type, accept_subtype);
  EXPECT_EQ(string{"*"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("application/*", accept_type, accept_subtype);
  EXPECT_EQ(string{"application"}, accept_type);
  EXPECT_EQ(string{"*"}, accept_subtype);
  parse_accept_header_item("application/json;q=0.8", accept_type, accept_subtype);
  EXPECT_EQ(string{"application"}, accept_type);
  EXPECT_EQ(string{"json"}, accept_subtype);
}

TEST(helper, check_accept_header) {
  const std::string accept_header{
      "image/gif, image/jpeg, image/pjpeg, application/x-ms-application, application/vnd.ms-xpsdocument, "
      "application/xaml+xml, application/x-ms-xbap, application/x-shockwave-flash, application/x-silverlight-2-b2, "
      "application/x-silverlight, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword,"
      "application/xml,application/xhtml+xml,text/html;q=0.9, text/plain;q=0.8, image/png"};

  EXPECT_TRUE(check_accept_header(accept_header, "*/*"));
  EXPECT_FALSE(check_accept_header(accept_header, "*/jpeg"));
  
  EXPECT_TRUE(check_accept_header(accept_header, "image/*"));
  EXPECT_TRUE(check_accept_header(accept_header, "image/jpeg"));
  EXPECT_TRUE(check_accept_header(accept_header, "image/*;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, "image/jpeg;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, "image/jpeg  ;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, " image /  *  ;q=0.5"));
  EXPECT_FALSE(check_accept_header(accept_header, "image/jpegs"));
  EXPECT_FALSE(check_accept_header(accept_header, "images/jpeg"));
  EXPECT_FALSE(check_accept_header(accept_header, " images /  *  ;q=0.5"));
  
  EXPECT_TRUE(check_accept_header(accept_header, "text/*"));
  EXPECT_TRUE(check_accept_header(accept_header, "text/plain"));
  EXPECT_TRUE(check_accept_header(accept_header, "text/*;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, "text/plain;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, "text/plain  ;q=0.5"));
  EXPECT_TRUE(check_accept_header(accept_header, " text /  *  ;q=0.5"));
  EXPECT_FALSE(check_accept_header(accept_header, "text/plains"));
  EXPECT_FALSE(check_accept_header(accept_header, "texts/plain"));
  EXPECT_FALSE(check_accept_header(accept_header, " texts /  *  ;q=0.5"));
}
