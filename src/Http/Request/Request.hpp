//
// Created by Evgeny Oyun on 07.01.17.
//

#ifndef HTTPD_REQUEST_H
#define HTTPD_REQUEST_H

#include "../Headers/Headers.hpp"

namespace Http
{
  class Request : public Headers {
  public:
    const string dump() const;
  };
}

#endif //HTTPD_REQUEST_H
