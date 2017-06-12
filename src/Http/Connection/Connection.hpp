//
// Created by Evgeny Oyun on 08.01.17.
//

#ifndef _HTTP_CONNECTION_H
#define _HTTP_CONNECTION_H

#include "../Http.hpp"
#include "../Parser/Cookie.cpp"

#include "../../../deps/httpd/httpd.h"

namespace Http
{
  using namespace std;

  class Connection{
    private:
      Request       _request;
      Response      _response;
    public:
      Request        *request;  // reference to self _request
      Response       *response; // reference to self _response
      httpd_string_t  httpd_buffer;
      string          buffer;
      uint8_t         state;

      Connection();
    };
}

#endif //_HTTP_CONNECTION_H
