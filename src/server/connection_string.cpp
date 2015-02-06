// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/01/2015
// Description: The Connection_String implementation which is an abstraction class for postgresql connection strings.
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

#include "connection_string.hpp"
#include "db_exception.hpp"

#include <algorithm>
#include <cctype>
#include <functional>

namespace {
void trim_left(string &str) {
  string::iterator stop_position{std::find_if_not(str.begin(), str.end(), std::ptr_fun<int, int>(std::isspace))};
  if (stop_position != str.end()) str.erase(str.begin(), stop_position);
}
void trim_right(string &str) {
  string::reverse_iterator start_position{
      std::find_if_not(str.rbegin(), str.rend(), std::ptr_fun<int, int>(std::isspace))};
  if (start_position != str.rend()) str.erase(start_position.base(), str.end());
}

void trim(string &str) {
  trim_left(str);
  trim_right(str);
}
}  // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace server {
const string Connection_String::DEFAULT_USER{"postgres"};
const string Connection_String::DEFAULT_HOST_ADDR{"127.0.0.1"};
const string Connection_String::DEFAULT_HOST{"localhost"};
const string Connection_String::DEFAULT_PASSWORD{""};
const string Connection_String::DEFAULT_DB_NAME{"translate"};
const unsigned short Connection_String::DEFAULT_PORT{5432};
const Connection_String::SSL_Mode Connection_String::DEFAULT_SSLMODE{Connection_String::SSL_Mode::PREFER};
const bool Connection_String::DEFAULT_REQUIRE_SSL{false};
const bool Connection_String::DEFAULT_KEEP_ALIVES{true};

Connection_String::Connection_String()
    : user_{DEFAULT_USER},
      host_{""},
      host_addr_{DEFAULT_HOST_ADDR},
      password_{DEFAULT_PASSWORD},
      db_name_{DEFAULT_DB_NAME},
      options_{""},
      client_encoding_{""},
      application_name_{""},
      fallback_application_name_{""},
      ssl_cert_{""},
      ssl_key_{""},
      ssl_root_cert_{""},
      ssl_crl_{""},
      require_peer_{""},
      krb_srv_name_{""},
      gss_lib_{""},
      service_{""},
      connect_timeout_{-1},
      keep_alives_idle_{-1},
      keep_alives_interval_{-1},
      keep_alives_count_{-1},
      ssl_mode_{DEFAULT_SSLMODE},
      port_{DEFAULT_PORT},
      keep_alives_{DEFAULT_KEEP_ALIVES},
      require_ssl_{DEFAULT_REQUIRE_SSL} {}

string Connection_String::to_string() const noexcept {
  string result{};
  if (this->has_user()) result += "user=" + this->user_;
  if (this->has_host()) result += (result.empty() ? "" : " ") + string{"host="} + this->host_;
  if (this->has_hostaddr()) result += (result.empty() ? "" : " ") + string{"hostaddr="} + this->host_addr_;
  if (this->has_password()) result += (result.empty() ? "" : " ") + string{"password="} + this->password_;
  if (this->has_dbname()) result += (result.empty() ? "" : " ") + string{"dbname="} + this->db_name_;
  if (this->has_options()) result += (result.empty() ? "" : " ") + string{"options="} + this->options_;
  if (this->has_client_encoding())
    result += (result.empty() ? "" : " ") + string{"client_encoding="} + this->client_encoding_;
  if (this->has_application_name())
    result += (result.empty() ? "" : " ") + string{"application_name="} + this->application_name_;
  if (this->has_fallback_application_name())
    result += (result.empty() ? "" : " ") + string{"fallback_application_name="} + this->fallback_application_name_;
  if (this->has_sslcert()) result += (result.empty() ? "" : " ") + string{"sslcert="} + this->ssl_cert_;
  if (this->has_sslkey()) result += (result.empty() ? "" : " ") + string{"sslkey="} + this->ssl_key_;
  if (this->has_sslrootcert()) result += (result.empty() ? "" : " ") + string{"sslrootcert="} + this->ssl_root_cert_;
  if (this->has_sslcrl()) result += (result.empty() ? "" : " ") + string{"sslcrl="} + this->ssl_crl_;
  if (this->has_requirepeer()) result += (result.empty() ? "" : " ") + string{"requirepeer="} + this->require_peer_;
  if (this->has_krbsrvname()) result += (result.empty() ? "" : " ") + string{"krbsrvname="} + this->krb_srv_name_;
  if (this->has_gsslib()) result += (result.empty() ? "" : " ") + string{"gsslib="} + this->gss_lib_;
  if (this->has_service()) result += (result.empty() ? "" : " ") + string{"service="} + this->service_;
  if (this->has_connect_timeout())
    result += (result.empty() ? "" : " ") + string{"connect_timeout="} + std::to_string(this->connect_timeout_);
  if (this->has_keepalives_idle())
    result += (result.empty() ? "" : " ") + string{"keepalives_idle="} + std::to_string(this->keep_alives_idle_);
  if (this->has_keepalives_interval())
    result +=
        (result.empty() ? "" : " ") + string{"keepalives_interval="} + std::to_string(this->keep_alives_interval_);
  if (this->has_keepalives_count())
    result += (result.empty() ? "" : " ") + string{"keepalives_count="} + std::to_string(this->keep_alives_count_);
  if (this->has_sslmode())
    result +=
        (result.empty() ? "" : " ") + string{"sslmode="} + lgeorgieff::translate::server::to_string(this->ssl_mode_);
  if (this->has_port()) result += (result.empty() ? "" : " ") + string{"port="} + std::to_string(this->port_);
  if (this->has_keepalives())
    result += (result.empty() ? "" : " ") + string{"keepalives="} + string{this->keep_alives_ ? "1" : "0"};
  if (this->has_requiressl())
    result += (result.empty() ? "" : " ") + string{"requiressl="} + string{this->require_ssl_ ? "1" : "0"};
  return result;
}

string Connection_String::user() const noexcept { return this->user_; }

void Connection_String::user(const string &user) noexcept {
  this->user_ = user;
  trim(this->user_);
}

bool Connection_String::has_user() const noexcept { return !this->user_.empty(); }

string Connection_String::host() const noexcept { return this->host_; }

void Connection_String::host(const string &host) {
  if (this->has_hostaddr()) throw DB_Exception("Connection_String: cannot set host when host_addr is set!");
  this->host_ = host;
  trim(this->host_);
}

bool Connection_String::has_host() const noexcept { return !this->host_.empty(); }

string Connection_String::hostaddr() const noexcept { return this->host_addr_; }

void Connection_String::hostaddr(const string &host_addr) {
  if (this->has_host()) throw DB_Exception("Connection_String: cannot set host_addr when host is set!");
  this->host_addr_ = host_addr;
  trim(this->host_);
}

bool Connection_String::has_hostaddr() const noexcept { return !this->host_addr_.empty(); }

string Connection_String::password() const noexcept { return this->password_; }

void Connection_String::password(const string &password) noexcept {
  this->password_ = password;
  trim(this->password_);
}

bool Connection_String::has_password() const noexcept { return !this->password_.empty(); }

string Connection_String::dbname() const noexcept { return this->db_name_; }

void Connection_String::dbname(const string &dbname) noexcept {
  this->db_name_ = dbname;
  trim(this->db_name_);
}

bool Connection_String::has_dbname() const noexcept { return !this->db_name_.empty(); }

string Connection_String::options() const noexcept { return this->options_; }

void Connection_String::options(const string &options) noexcept {
  this->options_ = options;
  trim(this->options_);
}

bool Connection_String::has_options() const noexcept { return !this->options_.empty(); }

string Connection_String::client_encoding() const noexcept { return this->client_encoding_; }

void Connection_String::client_encoding(const string &client_encoding) noexcept {
  this->client_encoding_ = client_encoding;
  trim(this->client_encoding_);
}

bool Connection_String::has_client_encoding() const noexcept { return !this->client_encoding_.empty(); }

string Connection_String::application_name() const noexcept { return this->application_name_; }

void Connection_String::application_name(const string &application_name) noexcept {
  this->application_name_ = application_name;
  trim(this->application_name_);
}

bool Connection_String::has_application_name() const noexcept { return !this->application_name_.empty(); }

string Connection_String::fallback_application_name() const noexcept { return this->fallback_application_name_; }

void Connection_String::fallback_application_name(const string &fallback_application_name) noexcept {
  this->fallback_application_name_ = fallback_application_name;
  trim(this->fallback_application_name_);
}

bool Connection_String::has_fallback_application_name() const noexcept {
  return !this->fallback_application_name_.empty();
}

string Connection_String::sslcert() const noexcept { return this->ssl_cert_; }

void Connection_String::sslcert(const string &sslcert) noexcept {
  this->ssl_cert_ = sslcert;
  trim(this->ssl_cert_);
}

bool Connection_String::has_sslcert() const noexcept { return !this->ssl_cert_.empty(); }

string Connection_String::sslkey() const noexcept { return this->ssl_key_; }

void Connection_String::sslkey(const string &sslkey) noexcept {
  this->ssl_key_ = sslkey;
  trim(this->ssl_key_);
}

bool Connection_String::has_sslkey() const noexcept { return !this->ssl_key_.empty(); }

string Connection_String::sslrootcert() const noexcept { return this->ssl_root_cert_; }

void Connection_String::sslrootcert(const string &sslrootcert) noexcept {
  this->ssl_root_cert_ = sslrootcert;
  trim(this->ssl_root_cert_);
}

bool Connection_String::has_sslrootcert() const noexcept { return !this->ssl_root_cert_.empty(); }

string Connection_String::sslcrl() const noexcept { return this->ssl_crl_; }

void Connection_String::sslcrl(const string &sslcrl) noexcept {
  this->ssl_crl_ = sslcrl;
  trim(this->ssl_crl_);
}

bool Connection_String::has_sslcrl() const noexcept { return !this->ssl_crl_.empty(); }

string Connection_String::requirepeer() const noexcept { return this->require_peer_; }

void Connection_String::requirepeer(const string &requirepeer) noexcept {
  this->require_peer_ = requirepeer;
  trim(this->require_peer_);
}

bool Connection_String::has_requirepeer() const noexcept { return !this->require_peer_.empty(); }

string Connection_String::krbsrvname() const noexcept { return this->krb_srv_name_; }

void Connection_String::krbsrvname(const string &krbsrvname) noexcept {
  this->krb_srv_name_ = krbsrvname;
  trim(this->krb_srv_name_);
}

bool Connection_String::has_krbsrvname() const noexcept { return !this->krb_srv_name_.empty(); }

string Connection_String::gsslib() const noexcept { return this->gss_lib_; }

void Connection_String::gsslib(const string &gsslib) noexcept {
  this->gss_lib_ = gsslib;
  trim(this->gss_lib_);
}

bool Connection_String::has_gsslib() const noexcept { return !this->gss_lib_.empty(); }

string Connection_String::service() const noexcept { return this->service_; }

void Connection_String::service(const string &service) noexcept {
  this->service_ = service;
  trim(this->service_);
}

bool Connection_String::has_service() const noexcept { return !this->service_.empty(); }

Connection_String::SSL_Mode Connection_String::sslmode() const noexcept { return this->ssl_mode_; }

void Connection_String::sslmode(const Connection_String::SSL_Mode &sslmode) noexcept {
  this->ssl_mode_ = sslmode;
  ssl_mode_set_ = true;
}

bool Connection_String::has_sslmode() const noexcept { return this->ssl_mode_set_; }

ssize_t Connection_String::connect_timeout() const noexcept { return this->connect_timeout_; }

void Connection_String::connect_timeout(const ssize_t &connect_timeout) noexcept {
  this->connect_timeout_ = connect_timeout;
}

bool Connection_String::has_connect_timeout() const noexcept { return this->connect_timeout_ >= 0; }

unsigned short Connection_String::port() const noexcept { return this->port_; }

void Connection_String::port(const unsigned short &port) noexcept { this->port_ = port; }

bool Connection_String::has_port() const noexcept { return this->port_ > 0; }

ssize_t Connection_String::keepalives_idle() const noexcept { return this->keep_alives_idle_; }

void Connection_String::keepalives_idle(const ssize_t &keepalives_idle) noexcept {
  this->keep_alives_idle_ = keepalives_idle;
}

bool Connection_String::has_keepalives_idle() const noexcept { return this->keep_alives_idle_ >= 0; }

ssize_t Connection_String::keepalives_interval() const noexcept { return this->keep_alives_interval_; }

void Connection_String::keepalives_interval(const ssize_t &keepalives_interval) noexcept {
  this->keep_alives_interval_ = keepalives_interval;
}

bool Connection_String::has_keepalives_interval() const noexcept { return this->keep_alives_interval_ >= 0; }

ssize_t Connection_String::keepalives_count() const noexcept { return this->keep_alives_count_; }

void Connection_String::keepalives_count(const ssize_t &keepalives_count) noexcept {
  this->keep_alives_count_ = keepalives_count;
}

bool Connection_String::has_keepalives_count() const noexcept { return this->keep_alives_count_ >= 0; }

bool Connection_String::keepalives() const noexcept { return this->keep_alives_; }

void Connection_String::keepalives(const bool &keepalives) noexcept {
  this->keep_alives_ = keepalives;
  this->keep_alives_set_ = true;
}

bool Connection_String::has_keepalives() const noexcept { return this->keep_alives_set_; }

bool Connection_String::requiressl() const noexcept { return this->require_ssl_; }

void Connection_String::requiressl(const bool &requiressl) noexcept {
  this->require_ssl_ = requiressl;
  this->require_ssl_set_ = true;
}

bool Connection_String::has_requiressl() const noexcept { return this->require_ssl_set_; }

ostream &operator<<(ostream &os, const Connection_String &connection_string) {
  os << connection_string.to_string();
  return os;
}

string to_string(const Connection_String::SSL_Mode &sslmode) {
  switch (sslmode) {
    case Connection_String::SSL_Mode::DISABLE:
      return "disable";
    case Connection_String::SSL_Mode::ALLOW:
      return "allow";
    case Connection_String::SSL_Mode::PREFER:
      return "prefer";
    case Connection_String::SSL_Mode::REQUIRE:
      return "require";
    case Connection_String::SSL_Mode::VERIFY_CA:
      return "verify-ca";
    case Connection_String::SSL_Mode::VERIFY_FULL:
      return "verify-full";
    default:
      throw DB_Exception("sslmode \"" + std::to_string(static_cast<char>(sslmode)) + "\" is invalid!");
  }
}

Connection_String::SSL_Mode from_string(const string &sslmode) {
  if (sslmode == "disable") return Connection_String::SSL_Mode::DISABLE;
  if (sslmode == "allow") return Connection_String::SSL_Mode::ALLOW;
  if (sslmode == "prefer") return Connection_String::SSL_Mode::PREFER;
  if (sslmode == "require") return Connection_String::SSL_Mode::REQUIRE;
  if (sslmode == "verify-ca") return Connection_String::SSL_Mode::VERIFY_CA;
  if (sslmode == "verify_full") return Connection_String::SSL_Mode::VERIFY_FULL;
  throw DB_Exception("sslmode \"" + sslmode + "\" is invalid!");
}

ostream &operator<<(ostream &os, const Connection_String::SSL_Mode &sslmode) {
  os << to_string(sslmode);
  return os;
}
}  // server
}  // translate
}  // lgeorgieff
