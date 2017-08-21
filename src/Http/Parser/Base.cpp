#include <iostream>
#include <functional>
#include "../Exception/Exception.hpp"
#ifndef _HTTP_PARSER_BASE_C
#define _HTTP_PARSER_BASE_C

namespace Http
{
  namespace Parser
  {
    using namespace std;

    class Base{
    public:
      static void parse(const string *raw, const char strings_delimiter, const char tokens_delimiter, function<void(const string*, const string*)> &handler)
      {
        if(raw == NULL || raw->size() == 0)
        {
          return;
        }

        //
        enum state_t{
            STATE_KEY_INIT  = 0,
            STATE_KEY_START = 1,
            STATE_KEY_END   = 2
        };

        state_t state  = STATE_KEY_INIT;
        size_t  i      = 0,
                offset = 0,
                length = 0,
                max    = raw->size() - 1;

        string key, value;

        for(i = 0; i<= max; i++)
        {
          switch(state)
          {
            case STATE_KEY_INIT: {
              if(raw->at(i) == ' ' || raw->at(i) == strings_delimiter)
              {
                offset++;
                continue;
              }

              state = STATE_KEY_START;
              length++;
            }
            break;
            case STATE_KEY_START:{
              if(raw->at(i) == ' ')
              {
                offset++;
                continue;
              }

              if(raw->at(i) == tokens_delimiter)
              {
                key = raw->substr(offset, length);

                state   = STATE_KEY_END;
                offset  = i;
                length  = 0;

                continue;
              }

              length++;
            }
            break;
            case STATE_KEY_END:{
              if(raw->at(i) == ' ')
              {
                offset++;
                continue;
              }

              if(raw->at(i) == strings_delimiter || i == max)
              {
                offset++;

                if(i == max && raw->at(max) != delimiter)
                {
                  length++;
                }

                value  = raw->substr(offset, length);

                handler(&key, &value);

                length = 0;
                offset = i + 1;
                state  = STATE_KEY_INIT;

                continue;
              }

              length++;
            }
            break;
          }
        }
      }
    };
  }
}

#endif // _HTTP_PARSER_BASE_C
