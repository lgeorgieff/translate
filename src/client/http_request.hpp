// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/06/2015
// Description: Declares the base class for an HTTP request to the translation service.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if/ not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================

#ifndef HTTP_REQUEST_HPP_
#define HTTP_REQUEST_HPP_

#include <curl/curl.h>

#include <cstddef>
#include <string>

namespace lgeorgieff {
namespace translate {
namespace client {
class HttpRequest {
 public:
  static const std::string DEFAULT_ACCEPT_HEADER;

  // === Constructors, destructor, operators ==========================================================================
  // The base constructor that takes the HTTP request's URL and optionally the accept header.
  explicit HttpRequest(const std::string&, const std::string& = DEFAULT_ACCEPT_HEADER);
  HttpRequest(const HttpRequest&);
  HttpRequest(HttpRequest&&);
  virtual ~HttpRequest();
  HttpRequest& operator=(const HttpRequest&);
  HttpRequest& operator=(HttpRequest&&);
  bool operator==(const HttpRequest&);
  bool operator!=(const HttpRequest&);
  // The actual method that handles the curl HTTP request and must be implemented in each derived class.
  // This method may throw a lgeorgieff::translate::utils::HttpException
  virtual std::string operator()() = 0;

  // A getter for the HTTP result as string value
  std::string result() const noexcept;
  // A getter for the HTTP status code.
  // If no HTTP request was completed so far, the return value is -1
  int status_code() const noexcept;
  // A getter for the request URL
  std::string url() const noexcept;
  // A getter for the accept header
  std::string accept_header() const noexcept;

  // A helper for initializing curl
  static CURLcode init_curl();
  // A helper for cleaning curl
  static void cleanup_curl();

 protected:
  std::string url_;
  std::string result_;
  std::string accept_header_;
  int status_code_;
  // The callback function that is called by curl during the HTTP request
  static size_t curl_write_(void*, size_t, size_t, void*);
};  // HttpRequest
}  // client
}  // translate
}  // lgeorgieff

#endif  // HTTP_REQUEST_HPP_
