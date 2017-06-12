//
// Created by Evgeny Oyun on 07.01.17.
//

#ifndef HTTPD_REQUEST_C
#define HTTPD_REQUEST_C

#include "Request.hpp"

namespace Http
{
  using namespace std;

  const string Request::dump() const
  {
    string _tmp;

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

    return _tmp;
  }
}

#endif // HTTPD_REQUEST_C
