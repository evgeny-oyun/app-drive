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
    using namespace Http::Parser;

    class Cookie{
    public:
      static void parse(const string *raw, const function<void(const string*, const string*)> &handler)
      {
        Base::parse(raw, ';', '=', handler);
      }
    };
  }
}

#endif // _HTTP_PARSER_COOKIE_C
