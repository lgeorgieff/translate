// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/04/2015
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
  http_request->result_.append((char *)ptr, size * nmemb);
  return size * nmemb;
}

CURLcode HttpRequest::init_curl() { return curl_global_init(CURL_GLOBAL_ALL); }

void HttpRequest::cleanup_curl() { curl_global_cleanup(); }

HttpRequest::HttpRequest(const std::string &url) : url_{url}, result_{}, status_code_{-1} {}

HttpRequest::HttpRequest(const HttpRequest &other)
    : url_{other.url_}, result_{other.result_}, status_code_{other.status_code_} {}

HttpRequest::HttpRequest(HttpRequest &&other)
    : url_{std::move(other.url_)},
      result_{std::move(other.result_)},
      status_code_{std::move(other.status_code_)} {}

HttpRequest::~HttpRequest() {}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
  this->url_ = other.url_;
  this->result_ = other.result_;
  this->status_code_ = other.status_code_;
  return *this;
}

HttpRequest &HttpRequest::operator=(HttpRequest &&other) {
  this->url_ = std::move(other.url_);
  this->result_ = std::move(other.result_);
  this->status_code_ = std::move(other.status_code_);
  return *this;
}

bool HttpRequest::operator==(const HttpRequest &other) {
  return this->url_ == other.url_ && this->result_ == other.result_ &&
         this->status_code_ == other.status_code_;
}

bool HttpRequest::operator!=(const HttpRequest &other) { return !(*this == other); }

std::string HttpRequest::result() const noexcept { return this->result_; }

int HttpRequest::status_code() const noexcept { return this->status_code_; }

std::string HttpRequest::url() const noexcept { return this->url_; }
}  // client
}  // translate
}  // lgeorgieff
