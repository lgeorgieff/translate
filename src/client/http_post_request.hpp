// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
// Description: Declares a class for an HTTP POST request to the translation service.
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

#ifndef HTTP_POST_REQUEST_HPP_
#define HTTP_POST_REQUEST_HPP_

#include "http_request.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace client {
class HttpPostRequest : public HttpRequest {
 public:
  static const std::string DEFAULT_CONTENT_TYPE_HEADER;

  // === Constructors and destructor ==================================================================================
  // ctor takes the HTTP request URL, POST data, accept header and content type header
  explicit HttpPostRequest(const std::string&, const std::string&,
                           const std::string& = HttpRequest::DEFAULT_ACCEPT_HEADER,
                           const std::string& = DEFAULT_CONTENT_TYPE_HEADER);
  virtual ~HttpPostRequest();

  // The actual method that handles the curl HTTP request and must be implemented in each derived class from
  // HttpRequest
  std::string operator()() override;
  // A getter for the content type header
  std::string content_type_header() const noexcept;

 protected:
  std::string post_data_;
  std::string content_type_header_;
};  // HttpPostRequest
}  // client
}  // translate
}  // lgeorgieff

#endif  // HTTP_POST_REQUEST_HPP_
