// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
// Description: Implements a class for an HTTP POST request to the translation service.
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

#include "http_post_request.hpp"
#include "utils/http_exception.hpp"

namespace lgeorgieff {
namespace translate {
namespace client {

using lgeorgieff::translate::utils::HttpException;

const std::string HttpPostRequest::DEFAULT_CONTENT_TYPE_HEADER{"application/json"};

HttpPostRequest::HttpPostRequest(const std::string &url, const std::string &post_data,
                                 const std::string &accept_header, const std::string &content_type_header)
    : HttpRequest{url, accept_header}, post_data_{post_data}, content_type_header_{content_type_header} {}

std::string HttpPostRequest::operator()() {
  CURL *curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, this->url_.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, this->post_data_.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &curl_write_);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, this);

  struct curl_slist *headers = nullptr;
  std::string accept_header{"Accept: " + this->accept_header_};
  std::string content_type_header{"Content-Type: " + this->content_type_header_};
  headers = curl_slist_append(headers, accept_header.c_str());
  headers = curl_slist_append(headers, content_type_header.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

  CURLcode curl_code{curl_easy_perform(curl_handle)};
  if (CURLE_OK != curl_code) {
    throw HttpException{"Failed to execute HTTP POST request: curl code " + std::to_string(curl_code) + " (" +
                        curl_easy_strerror(curl_code) + ")"};
  }

  curl_code = curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &this->status_code_);
  if (CURLE_OK != curl_code) {
    throw HttpException{"Failed to complete HTTP POST request: curl code " + std::to_string(curl_code) + " (" +
                        curl_easy_strerror(curl_code) + ")"};
  }

  curl_easy_cleanup(curl_handle);
  return this->result_;
}

HttpPostRequest::~HttpPostRequest() {}
}  // client
}  // translate
}  // lgeorgieff
