#ifndef _HTTP_PARSER_HTTP_C
#define _HTTP_PARSER_HTTP_C

#include "../../../deps/httpparser/src/httpparser/request.h"
#include "../../../deps/httpparser/src/httpparser/httprequestparser.h"

namespace Http
{
  namespace Parser
  {
    using namespace std;
    using namespace httpparser;

    class HttpRequest{
    private:
      HttpRequestParser _parser;
    public:
      void parse(const char *chunk, const size_t length)
      {
        
      }
    };

    ::HttpRequestParser _parser;
  }
}

#endif // _HTTP_PARSER_HTTP_C
