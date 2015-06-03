// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/26/2015
// Description: Implements a class for an HTTP GET request to the translation service.
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

#include "http_get_request.hpp"
#include "utils/http_exception.hpp"

namespace lgeorgieff {
namespace translate {
namespace client {

using lgeorgieff::translate::utils::HttpException;

HttpGetRequest::HttpGetRequest(const std::string &url) : HttpRequest{url} {}

void HttpGetRequest::do_request() {
  CURL *curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, this->request_url_.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &curl_write_);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, this);

  CURLcode curl_code{curl_easy_perform(curl_handle)};
  if (CURLE_OK != curl_code) {
    throw HttpException{"Failed to execute HTTP GET request: curl code " + std::to_string(curl_code) + " (" +
                        curl_easy_strerror(curl_code) + ")"};
  }

  curl_code = curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &this->http_return_code_);
  if (CURLE_OK != curl_code) {
    throw HttpException{"Failed to complete HTTP GET request: curl code " + std::to_string(curl_code) + " (" +
                        curl_easy_strerror(curl_code) + ")"};
  }

  curl_easy_cleanup(curl_handle);
}

HttpGetRequest::~HttpGetRequest() {}
}  // client
}  // translate
}  // lgeorgieff
