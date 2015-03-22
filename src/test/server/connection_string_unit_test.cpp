// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/08/2015
// Description: Unit test for the connection_string class
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

#include "../../server/connection_string.hpp"
#include "../../server/db_exception.hpp"

#include <string>
#include <sstream>

using lgeorgieff::translate::server::Connection_String;
using lgeorgieff::translate::server::to_string;
using lgeorgieff::translate::server::from_string;
using lgeorgieff::translate::server::DB_Exception;
typedef lgeorgieff::translate::server::Connection_String::SSL_Mode SSL_Mode;
using std::string;
using std::stringstream;

TEST(ssl_mode, to_string) {
  EXPECT_EQ(string{"disable"}, to_string(SSL_Mode::DISABLE));
  EXPECT_EQ(string{"allow"}, to_string(SSL_Mode::ALLOW));
  EXPECT_EQ(string{"prefer"}, to_string(SSL_Mode::PREFER));
  EXPECT_EQ(string{"require"}, to_string(SSL_Mode::REQUIRE));
  EXPECT_EQ(string{"verify-ca"}, to_string(SSL_Mode::VERIFY_CA));
  EXPECT_EQ(string{"verify-full"}, to_string(SSL_Mode::VERIFY_FULL));
  EXPECT_THROW(to_string((SSL_Mode)'c'), DB_Exception);
  stringstream ss;
  ss << SSL_Mode::REQUIRE;
  EXPECT_EQ("require", ss.str());
}

TEST(ssl_mode, from_string) {
  EXPECT_EQ(SSL_Mode::DISABLE, from_string("disable"));
  EXPECT_EQ(SSL_Mode::ALLOW, from_string("allow"));
  EXPECT_EQ(SSL_Mode::PREFER, from_string("prefer"));
  EXPECT_EQ(SSL_Mode::REQUIRE, from_string("require"));
  EXPECT_EQ(SSL_Mode::VERIFY_CA, from_string("verify-ca"));
  EXPECT_EQ(SSL_Mode::VERIFY_FULL, from_string("verify-full"));
  EXPECT_THROW(from_string("Disable"), DB_Exception);
  EXPECT_THROW(from_string(""), DB_Exception);
}

TEST(connection_string, operators) {
  Connection_String cs_1;
  Connection_String cs_2{cs_1};
  Connection_String cs_3 = cs_1;
  const size_t CSS_DIM{3};
  Connection_String css[CSS_DIM] = {cs_1, cs_2, cs_3};
  for (Connection_String cs : css) {
    EXPECT_TRUE(cs.has_user());
    EXPECT_EQ(string{"postgres"}, cs.user());
    EXPECT_FALSE(cs.has_host());
    EXPECT_EQ(string{}, cs.host());
    EXPECT_TRUE(cs.has_hostaddr());
    EXPECT_EQ(string{"127.0.0.1"}, cs.hostaddr());
    EXPECT_FALSE(cs.has_password());
    EXPECT_EQ(string{}, cs.password());
    EXPECT_TRUE(cs.has_dbname());
    EXPECT_EQ(string{"translate"}, cs.dbname());
    EXPECT_FALSE(cs.has_options());
    EXPECT_EQ(string{}, cs.options());
    EXPECT_FALSE(cs.has_client_encoding());
    EXPECT_EQ(string{}, cs.client_encoding());
    EXPECT_FALSE(cs.has_application_name());
    EXPECT_EQ(string{}, cs.application_name());
    EXPECT_FALSE(cs.has_fallback_application_name());
    EXPECT_EQ(string{}, cs.fallback_application_name());
    EXPECT_FALSE(cs.has_sslcert());
    EXPECT_EQ(string{}, cs.sslcert());
    EXPECT_FALSE(cs.has_sslkey());
    EXPECT_EQ(string{}, cs.sslkey());
    EXPECT_FALSE(cs.has_sslrootcert());
    EXPECT_EQ(string{}, cs.sslrootcert());
    EXPECT_FALSE(cs.has_sslcrl());
    EXPECT_EQ(string{}, cs.sslcrl());
    EXPECT_FALSE(cs.has_requirepeer());
    EXPECT_EQ(string{}, cs.requirepeer());
    EXPECT_FALSE(cs.has_krbsrvname());
    EXPECT_EQ(string{}, cs.krbsrvname());
    EXPECT_FALSE(cs.has_gsslib());
    EXPECT_EQ(string{}, cs.gsslib());
    EXPECT_FALSE(cs.has_service());
    EXPECT_EQ(string{}, cs.service());
    EXPECT_FALSE(cs.has_connect_timeout());
    EXPECT_EQ(-1, cs.connect_timeout());
    EXPECT_FALSE(cs.has_keepalives_idle());
    EXPECT_EQ(-1, cs.keepalives_idle());
    EXPECT_FALSE(cs.has_keepalives_interval());
    EXPECT_EQ(-1, cs.keepalives_interval());
    EXPECT_FALSE(cs.has_keepalives_count());
    EXPECT_EQ(-1, cs.keepalives_count());
    EXPECT_FALSE(cs.has_sslmode());
    EXPECT_EQ(SSL_Mode::PREFER, cs.sslmode());
    EXPECT_FALSE(cs.has_port());
    EXPECT_EQ(-1, cs.port());
    EXPECT_FALSE(cs.has_keepalives());
    EXPECT_EQ(1, cs.keepalives());
    EXPECT_FALSE(cs.has_requiressl());
    EXPECT_EQ(0, cs.requiressl());
  }
  EXPECT_TRUE(cs_1 == cs_2);
  EXPECT_TRUE(cs_1 == cs_3);
  EXPECT_TRUE(cs_2 == cs_3);
  EXPECT_TRUE(cs_1 == cs_1);
  cs_2.application_name("my-app");
  EXPECT_TRUE(cs_1 != cs_2);
}

TEST(connection_string, defaults) {
  EXPECT_EQ(string{"postgres"}, Connection_String::DEFAULT_USER);
  EXPECT_EQ(string{"127.0.0.1"}, Connection_String::DEFAULT_HOSTADDR);
  EXPECT_EQ(string{"localhost"}, Connection_String::DEFAULT_HOST);
  EXPECT_EQ(string{}, Connection_String::DEFAULT_PASSWORD);
  EXPECT_EQ(string{"translate"}, Connection_String::DEFAULT_DBNAME);
  EXPECT_EQ(5432, Connection_String::DEFAULT_PORT);
  EXPECT_EQ(SSL_Mode::PREFER, Connection_String::DEFAULT_SSLMODE);
  EXPECT_EQ(false, Connection_String::DEFAULT_REQUIRESSL);
  EXPECT_EQ(true, Connection_String::DEFAULT_KEEPALIVES);
}

TEST(connection_string, host_addr_host_name) {
  Connection_String cs_1;
  EXPECT_EQ(string{"127.0.0.1"}, cs_1.hostaddr());
  EXPECT_TRUE(cs_1.has_hostaddr());
  EXPECT_EQ(string{}, cs_1.host());
  EXPECT_FALSE(cs_1.has_host());
  cs_1.hostaddr("");
  EXPECT_EQ(string{}, cs_1.hostaddr());
  EXPECT_FALSE(cs_1.has_hostaddr());
  EXPECT_EQ(string{}, cs_1.host());
  EXPECT_FALSE(cs_1.has_host());
  cs_1.host("\t\n\r my-host.com   ");
  EXPECT_EQ(string{}, cs_1.hostaddr());
  EXPECT_FALSE(cs_1.has_hostaddr());
  EXPECT_EQ(string{"my-host.com"}, cs_1.host());
  EXPECT_TRUE(cs_1.has_host());
  EXPECT_THROW(cs_1.hostaddr("123.12.12.1"), DB_Exception);
  cs_1.host("");
  cs_1.hostaddr("123.12.12.1");
  EXPECT_THROW(cs_1.host("my-host.com"), DB_Exception);
}

TEST(connection_string, to_string) {
  Connection_String cs_1;
  string str_1{"user=postgres hostaddr=127.0.0.1 dbname=translate"};
  EXPECT_EQ(str_1, cs_1.to_string());
  stringstream ss;
  ss << cs_1;
  EXPECT_EQ(str_1, ss.str());

  cs_1.user("   me\n");
  cs_1.hostaddr(" \t\t\r    \n   ");
  cs_1.host(" my-db-host.com   ");
  cs_1.password("my passwd ");
  cs_1.dbname("MyDB");
  cs_1.options("opt1=\"1 2 3\" opt2=\\'3\\'    \n");
  cs_1.client_encoding("  utf-8");
  cs_1.application_name("myApp");
  cs_1.fallback_application_name("myFallbackApp\n\n\n");
  cs_1.sslcert("    /tmp/path/to/file.cert\t");
  cs_1.sslkey("/tmp/path/to/file.cert");
  cs_1.sslrootcert("       file.root        ");
  cs_1.sslcrl("\t\t/tmp/file.crt");
  cs_1.requirepeer("\nme\n");
  cs_1.krbsrvname("my_krb_service");
  cs_1.gsslib("c:\\path\\to\\gss.lib");
  cs_1.service(" myService");
  cs_1.connect_timeout(3);
  cs_1.keepalives_idle(7);
  cs_1.keepalives_interval(11);
  cs_1.keepalives_count(9);
  cs_1.sslmode(SSL_Mode::REQUIRE);
  cs_1.port(2345);
  cs_1.keepalives(false);
  cs_1.requiressl(true);
  string str_2{
      "user=me host=my-db-host.com password='my passwd' dbname=MyDB options='opt1=\"1 2 3\" opt2=\\'3\\'' "
      "client_encoding=utf-8 application_name=myApp fallback_application_name=myFallbackApp "
      "sslcert=/tmp/path/to/file.cert sslkey=/tmp/path/to/file.cert sslrootcert=file.root sslcrl=/tmp/file.crt "
      "requirepeer=me krbsrvname=my_krb_service gsslib=c:\\path\\to\\gss.lib service=myService connect_timeout=3 "
      "keepalives_idle=7 keepalives_interval=11 keepalives_count=9 sslmode=require port=2345 keepalives=0 "
      "requiressl=1"};
  EXPECT_EQ(str_2, cs_1.to_string());

  Connection_String cs_2{cs_1};
  cs_1.options("'opt1=\"1 2 3\" opt2=3 '   \n");
  cs_2.gsslib("");
  cs_2.service(" \n\t\n     \t");
  cs_2.connect_timeout(0);
  cs_2.keepalives_idle(-1);
  cs_2.keepalives_interval(-2);
  cs_2.keepalives_count(-3);
  cs_2.sslmode(SSL_Mode::ALLOW);
  cs_2.port(0);
  cs_2.keepalives(Connection_String::DEFAULT_KEEPALIVES);
  cs_2.requiressl(Connection_String::DEFAULT_REQUIRESSL);
  string str_3{
      "user=me host=my-db-host.com password='my passwd' dbname=MyDB options='opt1=\"1 2 3\" opt2=3 ' "
      "client_encoding=utf-8 application_name=myApp fallback_application_name=myFallbackApp "
      "sslcert=/tmp/path/to/file.cert sslkey=/tmp/path/to/file.cert sslrootcert=file.root sslcrl=/tmp/file.crt "
      "requirepeer=me krbsrvname=my_krb_service connect_timeout=0 sslmode=allow keepalives=1 requiressl=0"};
}
