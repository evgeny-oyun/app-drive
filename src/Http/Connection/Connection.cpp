 //
// Created by Evgeny Oyun on 08.01.17.
//

#ifndef _HTTP_CONNECTION_C
#define _HTTP_CONNECTION_C

#include "Connection.hpp"

namespace Http
{
  Connection::Connection()
  {
    request   = &_request;
    response  = &_response;
    state     = CONNECTION_STATE_REQUEST_READ;

    /* by default we are keep a connection on until response were ready */
    httpd_buffer.value  = NULL;
    httpd_buffer.length = 0;
  }
}
#endif //_HTTP_CONNECTION_C
