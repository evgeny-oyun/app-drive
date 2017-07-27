#ifndef _SYSTEM_UTILS_DATE_C
#define _SYSTEM_UTILS_DATE_C

#include <iostream>
#include <string>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdlib>

namespace System
{
  namespace Utils
  {
    class Date
    {
    public:
      static std::string timestamp(const char *fmt)
      {
        struct tm *timeinfo;
        time_t rawtime;
        char buffer[128];
        
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        memset(buffer, 0, sizeof(buffer));

        strftime(
          buffer, 
          sizeof(buffer), 
          fmt, 
          timeinfo
        );

        return std::string(buffer);
      }

      static std::string timestamp(time_t rawtime, const char *fmt)
      {
        struct tm *timeinfo;
        timeinfo = localtime(&rawtime);

        char buffer[128];

        memset(buffer, 0, sizeof(buffer));

        strftime(
          buffer, 
          sizeof(buffer), 
          fmt, 
          timeinfo
        );

        return std::string(buffer);
      }
    };
  }
}

#endif // _SYSTEM_UTILS_DATE_C
