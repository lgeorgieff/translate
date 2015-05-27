// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/27/2015
// Description: Implements the base class for an HTTP request to the translation service.
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

#include "http_request.hpp"

#include <utility>

namespace lgeorgieff {
namespace translate {
namespace client {

size_t HttpRequest::curl_write_(void *ptr, size_t size, size_t nmemb, void *user_data) {
  HttpRequest *http_request(static_cast<HttpRequest *>(user_data));
  http_request->http_result_.append((char *)ptr, size * nmemb);
  return size * nmemb;
}

CURLcode HttpRequest::init_curl() {
  return curl_global_init(CURL_GLOBAL_ALL);
}

void HttpRequest::cleanup_curl() {
  curl_global_cleanup();
}

HttpRequest::HttpRequest(const std::string &url) : request_url_{url}, http_result_{}, http_return_code_{-1} {}

HttpRequest::HttpRequest(const HttpRequest &other)
    : request_url_{other.request_url_}, http_result_{other.http_result_}, http_return_code_{other.http_return_code_} {}

HttpRequest::HttpRequest(HttpRequest &&other)
  : request_url_{std::move(other.request_url_)}, http_result_{std::move(other.http_result_)}, http_return_code_{std::move(other.http_return_code_)} {}

HttpRequest::~HttpRequest() {}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
  this->request_url_ = other.request_url_;
  this->http_result_ = other.http_result_;
  this->http_return_code_ = other.http_return_code_;
  return *this;
}

HttpRequest &HttpRequest::operator=(HttpRequest &&other) {
  this->request_url_ = std::move(other.request_url_);
  this->http_result_ = std::move(other.http_result_);
  this->http_return_code_ = std::move(other.http_return_code_);
  return *this;
}

bool HttpRequest::operator==(const HttpRequest &other) {
  return this->request_url_ == other.request_url_ && this->http_result_ == other.http_result_ &&
         this->http_return_code_ == other.http_return_code_;
}

bool HttpRequest::operator!=(const HttpRequest &other) { return !(*this == other); }

std::string HttpRequest::operator()() {
  this->do_request();
  return this->http_result_;
}

std::string HttpRequest::http_result() const noexcept { return this->http_result_; }

int HttpRequest::http_return_code() const noexcept { return this->http_return_code_; }
}  // client
}  // translate
}  // lgeorgieff
