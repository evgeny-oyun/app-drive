//
// Created by Evgeny Oyun on 07.01.17.
//
#ifndef HTTPD_RESPONSE_C
#define HTTPD_RESPONSE_C

#include "Response.hpp"

namespace Http
{
  using namespace std;

  void Response::setStatus(const int code)
  {
    _code = code;
  }

  void Response::clear()
  {
    _headers.clear();
    _cookies.clear();
    _buffer.clear();
    _body.clear();
  }

  const string Response::headers() const
  {
    string _tmp = "HTTP/1.0 ";
    _tmp += to_string(_code);
    _tmp += " ";
    _tmp += RESPONSE_CODES.at(_code);
    _tmp += "\r\n";

    // build header pairs
    if(_headers.size() > 0)
    {
      for (auto &h: _headers)
      {
        _tmp += h.first + ": " + h.second + "\r\n";
      }
    }

    // build cookies
    if(_cookies.size() > 0)
    {
      for(auto &c: _cookies)
      {
        _tmp += "Set-Cookie: " + c.first + "=" + c.second + "; path=/\r\n";
      }
    }

    // separator
    _tmp += "\r\n";

    return _tmp;
  }
}

#endif // HTTPD_RESPONSE_C
