#ifndef _HTTP_SERVER_C
#define _HTTP_SERVER_C

//
#include "Server.hpp"
#include "../Exception/Exception.hpp"
#include "../Headers/Headers.cpp"
#include "../Response/Response.cpp"
#include "../Request/Request.cpp"
#include "../Connection/Connection.cpp"

#include "../../../deps/httpd/httpd.c"

namespace Http {
  using namespace std;

  static void *on_init(void *httpd_data)
  {
    Connection *c = new Connection();

    return (void*)c;
  }

  static void on_header(const httpd_string_t *name, const httpd_string_t *value, void *httpd_data, void *data)
  {
    Connection *c = (Connection*)data;

    if(c == NULL || name == NULL || value == NULL || name->value == NULL || value->value == NULL)
    {
      #ifdef APP_DRIVE_DEBUG
      throw Exception("on_header(): data pointer is NULL");
      #endif
      return;
    }

    c->request->setHeader(
      string(name->value,   name->length),
      string(value->value,  value->length)
    );
  }

  static void on_body(const httpd_string_t *chunk, void *httpd_data, void *data)
  {
    Connection *c = (Connection*)data;

    if(c == NULL || chunk == NULL || chunk->value == NULL)
    {
      #ifdef APP_DRIVE_DEBUG
      throw Exception("on_header(): data pointer is NULL");
      #endif
      return;
    }

    c->request->write(string(chunk->value, chunk->length));
  }

  static void on_error(httpd_error_t error_code, void *httpd_data, void *data)
  {
    Connection *connection = (Connection*)data;

    if(connection == NULL)
    {
      return;
    }

    connection->state = CONNECTION_STATE_REQUEST_ERROR;

    switch(error_code)
    {
      case HTTPD_ERR_IO_READ:{
        throw Exception("i/o read() error");
      }
      break;
      case HTTPD_ERR_IO_WRITE:{
        throw Exception("i/o write() error");
      }
      break;
      case HTTPD_ERR_IO_CLOSE:{
        throw Exception("connection refused");
      }
      break;
      case HTTPD_ERR_PARSER:{
        throw Exception("HTTP request parsing failed");
      }
      break;
    }
  }

  static httpd_string_t *on_complete(void *httpd_data, void *data)
  {
    Server            *server     = (Server*)httpd_data;
    Connection        *connection = (Connection*)data;

    if(server == NULL || connection == NULL)
    {
      return NULL;
    }

    if(connection->state <= CONNECTION_STATE_REQUEST_PROCESS)
    {
      if(server->callback(connection->request, connection->response) == true)
      {
        connection->state = CONNECTION_STATE_RESPONSE_HEADERS;
      }
      else
      {
        connection->state = CONNECTION_STATE_REQUEST_PROCESS;
      }
    }

    switch(connection->state)
    {
      case CONNECTION_STATE_RESPONSE_HEADERS:{
        connection->buffer = connection->response->headers();

        connection->httpd_buffer.value  = (char*)connection->buffer.c_str();
        connection->httpd_buffer.length = connection->buffer.size();

        connection->state = CONNECTION_STATE_RESPONSE_BODY;
      }
      break;
      case CONNECTION_STATE_RESPONSE_BODY:{
        connection->buffer = connection->response->body();

        if(connection->buffer.size() == 0)
        {
          connection->state = CONNECTION_STATE_RESPONSE_COMPLETED;

          connection->httpd_buffer.value = NULL;
          connection->httpd_buffer.length = 0;
        }
        else
        {
          connection->httpd_buffer.value  = (char*)connection->buffer.c_str();
          connection->httpd_buffer.length = connection->buffer.size();

          connection->state = CONNECTION_STATE_RESPONSE_COMPLETED;
        }
      }
      break;
      case CONNECTION_STATE_RESPONSE_COMPLETED:{
        delete connection;
        return NULL;
      }
      break;
    }

    return &connection->httpd_buffer;
  }

  void Server::bind(const char *listen, const server_callback_t &_callback)
  {
    callback = _callback;

    httpd_handlers_init(&handlers);

    handlers.on_init      = &on_init;
    handlers.on_header    = &on_header;
    handlers.on_body      = &on_body;
    handlers.on_error     = &on_error;
    handlers.on_complete  = &on_complete;

    httpd = httpd_bind(
      listen,
      &handlers,
      (void*)this
    );

    if(httpd == NULL)
    {
      throw runtime_error("HTTP server binding failed");
      return;
    }
  }

  void Server::loop()
  {
    httpd_loop();
  }
}

#endif // _HTTP_SERVER_C
