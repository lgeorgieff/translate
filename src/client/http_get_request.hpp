// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/04/2015
// Description: Declares a class for an HTTP GET request to the translation service.
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

#ifndef HTTP_GET_REQUEST_HPP_
#define HTTP_GET_REQUEST_HPP_

#include "http_request.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace client {
class HttpGetRequest : public HttpRequest {
 public:
  // === Constructors and destructor ==================================================================================
  explicit HttpGetRequest(const std::string &);
  virtual ~HttpGetRequest();

  // The actual method that handles the curl HTTP request and must be implemented in each derived class from
  // HttpRequest
  std::string operator()() override;
};  // HttpGetRequest
}  // client
}  // translate
}  // lgeorgieff

#endif  // HTTP_GET_REQUEST_HPP_
