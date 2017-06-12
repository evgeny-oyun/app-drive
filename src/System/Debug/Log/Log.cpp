#ifndef _APPLICATION_LOG_C
#define _APPLICATION_LOG_C

#include "Log.hpp"

namespace System{
  namespace Debug{
    Log::Log()
    {
      buffer      = (char*)calloc(1, 3968);
      date_buffer = (char*)calloc(1, 128);
    }

    Log::~Log()
    {
      free(buffer);
      free(date_buffer);
      free(path);
    }

    void Log::init(const char *path_)
    {
      path = strdup(path_);
    }

    void Log::write(log_t type, const char *message, ...)
    {
      struct tm   *timeinfo;
      time_t       rawtime;
      size_t       date_buffer_length = 0;
      size_t       buffer_length = 0;
      va_list      arg_list;



    	if(type < LOG_DEBUG && type > LOG_FATAL)
    	{
    		return;
    	}

    	memset(date_buffer, 0, 128);
    	memset(buffer, 0, 3968);

      time(&rawtime);

      timeinfo = localtime(&rawtime);

      date_buffer_length = strftime(
    		date_buffer,
    		128,
    		"%d-%m-%Y %H:%M:%S",
    		timeinfo
    	);

      if(date_buffer_length > 0)
      {
        date_buffer[date_buffer_length]='\0';

        va_start(arg_list, message);
        buffer_length = vsnprintf(buffer, 3968, message, arg_list);
        va_end(arg_list);

        if(buffer_length > 0)
        {
          buffer[buffer_length] = '\0';

          FILE *fd = fopen(this->path, "a+");

          if(fd != NULL)
          {
            fprintf(
            	fd,
            	"%s | %s: %s\n",
              date_buffer,
              types[type],
              buffer
            );

            fclose(fd);
          }
        }
      }
    }

    void Log::write(log_t type, std::string message)
    {
      this->write(type, message.c_str());
    }
  }
}
#endif // _APPLICATION_LOG_C
