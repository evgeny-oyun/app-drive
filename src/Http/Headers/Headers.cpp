//
// Created by Evgeny Oyun on 07.01.17.
//

#include "Headers.hpp"

#ifndef _HTTPD_HEADERS_C
#define _HTTPD_HEADERS_C

namespace Http
{
  using namespace std;

  void Headers::setHeader(const string &name, const int &value)
  {
    setHeader(name, to_string(value));
  }

  void Headers::setHeader(const string &name, const string &value)
  {
    _headers[name] = value;
  }

  std::string Headers::getHeader(const std::string name)
  {
    return _headers[name];
  }

  void Headers::setCookie(const string &name, const string &value)
  {
    _cookies[name] = value;
  }

  void Headers::setCookie(const string *name, const string *value)
  {
    setCookie(*name, *value);
  }

  string Headers::getCookie(const string name)
  {
    return _cookies[name];
  }

  void Headers::operator<<(const string &msg)
  {
    _body += msg;
  }

  void Headers::write(const string &msg)
  {
    _body += msg;
  }

  const string Headers::body() const
  {
    return _body;
  }
}

#endif // _HTTPD_HEADERS_C
