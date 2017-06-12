#ifndef _APPLICATION_LOG_H
#define _APPLICATION_LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

namespace System{
  namespace Debug{
    typedef enum {
      LOG_DEBUG     = 0,
      LOG_NOTICE    = 1,
      LOG_WARNING   = 2,
      LOG_ERROR     = 3,
      LOG_FATAL     = 4
    } log_t;

    const char *types[] = {
      "Debug",
      "Notice",
      "Warning",
      "Error",
      "Fatal"
    };

    class Log{
    private:
      char *buffer;
      char *date_buffer;
      char *path;
    public:
      Log();
      ~Log();
      void init(const char *path_);
      void write(log_t type, const char *message, ...);
      void write(log_t type, std::string message);
    };
  }
}

#endif // _APPLICATION_LOG_H
