//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
namespace server {

/// A request received from a client.
struct request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
  std::string content;

  bool findHeader(std::string headerName, header** hd) const
  {
        auto it = std::find(headers.begin(), headers.end(), header(headerName, ""));
        if(it != headers.end())
            *hd = const_cast<header*>(&(*it));
        return it != headers.end();
  }

};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP
