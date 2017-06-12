#include <iostream>
#include <string>
#include <functional>
#include <exception>

#ifndef _HTTP_EXCEPTION_C
#define _HTTP_EXCEPTION_C

namespace Http
{
  using namespace std;

  class Exception{
    private:
      string msg;
    public:
      Exception(const string &message)
      {
        msg = message;
      }

      string what() const
      {
        return msg;
      }
  };
}

#endif // _HTTP_EXCEPTION_C
