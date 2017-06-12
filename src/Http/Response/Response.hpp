//
// Created by Evgeny Oyun on 07.01.17.
//


#ifndef HTTPD_RESPONSE_H
#define HTTPD_RESPONSE_H

#include "../Headers/Headers.hpp"

namespace Http
{
  using namespace std;

  class Response : public Headers {
    private:
      int     _code   = 200; // HTTP status code
      string  _buffer;
    public:
      void setStatus(const int code);
      void clear();
      const string headers() const;
  };
}

#endif //HTTPD_RESPONSE_H
