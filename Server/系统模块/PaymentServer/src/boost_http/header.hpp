//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include "stdafx.h"

namespace http {
namespace server {

struct header
{
  std::string name;
  std::string value;

  header()
      : name("")
      , value("")
  {  }

  header(const header& des)
  {
      *this = des;
  }

  header(std::string& name, std::string& value)
  {
      this->name = name;
      this->value = value;
  }

  header(std::string name, std::string value)
  {
      this->name = name;
      this->value = value;
  }

  header& operator = (const header &des)
  {
      name = des.name;
      value = des.value;
      return *this;
  }

  bool operator == (const std::string &des) const
  {
      std::string tmp1 = des;
      std::string tmp2 = name;

      boost::algorithm::to_lower(tmp1);
      boost::algorithm::to_lower(tmp2);

      return strcmp(tmp1.c_str(), tmp2.c_str()) == 0;
  }

  bool operator == (const header& des) const
  {
      std::string tmp1 = des.name;
      std::string tmp2 = name;

      boost::algorithm::to_lower(tmp1);
      boost::algorithm::to_lower(tmp2);

      return strcmp(tmp1.c_str(), tmp2.c_str()) == 0;
  }

};

} // namespace server
} // namespace http

#endif // HTTP_HEADER_HPP
