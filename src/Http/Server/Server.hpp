#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

//
#include "../Http.hpp"
#include "../Headers/Headers.hpp"
#include "../Response/Response.hpp"
#include "../Request/Request.hpp"
#include "../Connection/Connection.hpp"

#include "../../../deps/httpd/httpd.c"

#include <functional>
#include <unordered_map>

namespace Http
{
  using namespace std;

  typedef function<bool(Http::Request *req, Http::Response *res)> server_callback_t;

  class Server{
  private:
    httpd_t            *httpd;
    httpd_handlers_t    handlers;
  public:
    server_callback_t   callback;

    void bind(const char *listen, const server_callback_t &_callback);
    static void loop();
  };
}

#endif // _HTTP_SERVER_H
