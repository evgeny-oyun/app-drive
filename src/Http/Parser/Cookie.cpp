#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <stdexcept>

#include "Base.cpp"

#ifndef _HTTP_PARSER_COOKIE_C
#define _HTTP_PARSER_COOKIE_C

namespace Http
{
  namespace Parser
  {
    using namespace std;

    class Cookie{
    public:
      static void parse(const string *raw, function<void(const string*, const string*)> &handler)
      {
        Base(raw, ';', handler);
      }
    };
  }
}

#endif // _HTTP_PARSER_COOKIE_C
