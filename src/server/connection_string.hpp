// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: The ConnectionString declaration which is an abstraction class for postgresql connection strings.
//              More information to postgresql connetion string available at:
//              http://www.postgresql.org/docs/9.1/static/libpq-connect.html
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

#ifndef CONNECTION_STRING_HPP_
#define CONNECTION_STRING_HPP_

#include <cstddef>
#include <string>
#include <ostream>

using std::string;
using std::ostream;

namespace lgeorgieff {
namespace translate {
namespace server {
class ConnectionString {
 public:
  enum class SSL_Mode : char { DISABLE = 1, ALLOW = 2, PREFER = 4, REQUIRE = 8, VERIFY_CA = 16, VERIFY_FULL = 32 };

 public:
  /// A constructor that takes all supported connection string attributes
  ConnectionString();
  ConnectionString(const ConnectionString&) = default;
  ConnectionString(ConnectionString&&) = default;

  ConnectionString& operator=(const ConnectionString&) = default;
  ConnectionString& operator=(ConnectionString&&) = default;
  bool operator==(const ConnectionString&);
  bool operator!=(const ConnectionString&);

  ~ConnectionString() = default;

  // The default value for user if no one is provided
  static const string DEFAULT_USER;
  // The default value for hostaddr if no one is provided
  static const string DEFAULT_HOSTADDR;
  // The default host name, if no one is provided
  static const string DEFAULT_HOST;
  // The default value for password if no one is provided
  static const string DEFAULT_PASSWORD;
  // The default value for dbname if no one is provided
  static const string DEFAULT_DBNAME;
  // The default value for port if no one is provided
  static const ssize_t DEFAULT_PORT;
  // The default sslmode if enabled and no one is provided
  static const SSL_Mode DEFAULT_SSLMODE;
  // The default requiressl if no one is provided
  static const bool DEFAULT_REQUIRESSL;
  // The default keepalives if no one is provided
  static const bool DEFAULT_KEEPALIVES;

  // Returns the connection string constructed of all given members of this class
  string to_string() const noexcept;

  // all getters and setters
  string user() const noexcept;
  void user(const string&) noexcept;
  bool has_user() const noexcept;
  string host() const noexcept;
  void host(const string&);
  bool has_host() const noexcept;
  string hostaddr() const noexcept;
  void hostaddr(const string&);
  bool has_hostaddr() const noexcept;
  string password() const noexcept;
  void password(const string&) noexcept;
  bool has_password() const noexcept;
  string dbname() const noexcept;
  void dbname(const string&) noexcept;
  bool has_dbname() const noexcept;
  string options() const noexcept;
  void options(const string&) noexcept;
  bool has_options() const noexcept;
  string client_encoding() const noexcept;
  void client_encoding(const string&) noexcept;
  bool has_client_encoding() const noexcept;
  string application_name() const noexcept;
  void application_name(const string&) noexcept;
  bool has_application_name() const noexcept;
  string fallback_application_name() const noexcept;
  void fallback_application_name(const string&) noexcept;
  bool has_fallback_application_name() const noexcept;
  string sslcert() const noexcept;
  void sslcert(const string&) noexcept;
  bool has_sslcert() const noexcept;
  string sslkey() const noexcept;
  void sslkey(const string&) noexcept;
  bool has_sslkey() const noexcept;
  string sslrootcert() const noexcept;
  void sslrootcert(const string&) noexcept;
  bool has_sslrootcert() const noexcept;
  string sslcrl() const noexcept;
  void sslcrl(const string&) noexcept;
  bool has_sslcrl() const noexcept;
  string requirepeer() const noexcept;
  void requirepeer(const string&) noexcept;
  bool has_requirepeer() const noexcept;
  string krbsrvname() const noexcept;
  void krbsrvname(const string&) noexcept;
  bool has_krbsrvname() const noexcept;
  string gsslib() const noexcept;
  void gsslib(const string&) noexcept;
  bool has_gsslib() const noexcept;
  string service() const noexcept;
  void service(const string&) noexcept;
  bool has_service() const noexcept;
  SSL_Mode sslmode() const noexcept;
  void sslmode(const SSL_Mode&) noexcept;
  bool has_sslmode() const noexcept;
  ssize_t connect_timeout() const noexcept;
  void connect_timeout(const ssize_t&) noexcept;
  bool has_connect_timeout() const noexcept;
  ssize_t port() const noexcept;
  void port(const ssize_t&) noexcept;
  bool has_port() const noexcept;
  ssize_t keepalives_idle() const noexcept;
  void keepalives_idle(const ssize_t&) noexcept;
  bool has_keepalives_idle() const noexcept;
  ssize_t keepalives_interval() const noexcept;
  void keepalives_interval(const ssize_t&) noexcept;
  bool has_keepalives_interval() const noexcept;
  ssize_t keepalives_count() const noexcept;
  void keepalives_count(const ssize_t&) noexcept;
  bool has_keepalives_count() const noexcept;
  bool keepalives() const noexcept;
  void keepalives(const bool&) noexcept;
  bool has_keepalives() const noexcept;
  bool requiressl() const noexcept;
  void requiressl(const bool&) noexcept;
  bool has_requiressl() const noexcept;

 private:
  string user_;
  string host_;
  string host_addr_;
  string password_;
  string db_name_;
  string options_;
  string client_encoding_;
  string application_name_;
  string fallback_application_name_;
  string ssl_cert_;
  string ssl_key_;
  string ssl_root_cert_;
  string ssl_crl_;
  string require_peer_;
  string krb_srv_name_;
  string gss_lib_;
  string service_;
  ssize_t connect_timeout_;
  ssize_t keep_alives_idle_;
  ssize_t keep_alives_interval_;
  ssize_t keep_alives_count_;
  ssize_t port_;
  SSL_Mode ssl_mode_;
  bool keep_alives_;
  bool keep_alives_set_;
  bool require_ssl_;
  bool require_ssl_set_;
  bool ssl_mode_set_;
};  // ConnectionString

ostream& operator<<(ostream&, const ConnectionString&);

string to_string(const ConnectionString::SSL_Mode&);
ConnectionString::SSL_Mode from_string(const string&);
ostream& operator<<(ostream&, const ConnectionString::SSL_Mode&);
}  // server
}  // translate
}  // lgeorgieff

#endif  // CONNECTION_STRING_HPP_
