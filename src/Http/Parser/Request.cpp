#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <stdexcept>

#include "Base.cpp"

#ifndef _HTTP_PARSER_REQUEST_C
#define _HTTP_PARSER_REQUEST_C

namespace Http
{
  namespace Parser
  {
    using namespace std;

    class Request{
    public:
      static void parse(const string *raw, const function<void(const string*, const string*)> &handler)
      {
        Base(raw, ':', '\n', handler);
      }
    };
  }
}

#endif // _HTTP_PARSER_REQUEST_C
