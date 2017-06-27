#ifndef _SYSTEM_SHELL_C
#define _SYSTEM_SHELL_C

#include <exception>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef SYSTEM_UTILS_SHELL_BUFFER_SIZE
#define SYSTEM_UTILS_SHELL_BUFFER_SIZE 4096
#endif

namespace System
{
  namespace Utils
  {
    using namespace std;
    class Shell{
    private:
      int          fd;
      FILE        *stream = NULL;
      std::string  _result;
      bool         _finished = false;
    public:
      Shell(const string &c)
      {
        Shell(c.c_str());
      }

      Shell(const char *_command)
      {
        if((stream = popen(_command, "r")) == NULL)
        {
          _finished = true;
          return;
        }

        if((fd = fileno(stream)) <= 0)
        {
          _finished = true;
          pclose(stream);
          return;
        }

        // set non-block read operation
        if ((fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK)) == -1)
        {
          _finished = true;
          pclose(stream);
          return;
        }
      }

      ~Shell()
      {
        if(stream != NULL)
        {
          pclose(stream);
        }
      }

      bool busy()
      {
        return (stream != NULL && fd > 0);
      }

      void execute()
      {
        if(_finished == true)
        {
          return;
        }

        char buffer[SYSTEM_UTILS_SHELL_BUFFER_SIZE];
        int read_bytes;

        memset(buffer, 0, SYSTEM_UTILS_SHELL_BUFFER_SIZE);

        read_bytes = read(fd, buffer, SYSTEM_UTILS_SHELL_BUFFER_SIZE);

        if(read_bytes == -1)
        {
          if(errno != EAGAIN || errno != EWOULDBLOCK)
          {
            close(fd);
            pclose(stream);
            _finished = true;
          }
        }
        else if(read_bytes == 0 || feof(stream))
        {
          close(fd);
          pclose(stream);
          _finished = true;
        }
        else
        {
          buffer[read_bytes] = '\0';
          _result += std::string(buffer, read_bytes);
        }
      }
      bool done()
      {
        return _finished;
      }

      std::string response() const
      {
        return _result;
      }
    };
  }
}

#endif // _SYSTEM_SHELL_C
