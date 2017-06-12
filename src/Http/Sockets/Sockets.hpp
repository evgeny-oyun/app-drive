#ifndef _HTTP_SOCKETS_H
#define _HTTP_SOCKETS_H

#include <stdexcept>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <pwd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>

//#include <sys/sendfile.h>

namespace Http {
  class Socket {
  private:
      int _socket;
      int _timeout;
      int _backlog;
      int _buffer_size;
      char *_address;

      //
      int _tcp(const char *l_host, const char *l_port);

      int _unix(const char *path);

      void _parse(const char *address);

  public:
      Socket(const char *address);

      static bool setNonBlock(int _fd);

      void setTimeout(int a);

      void setBacklogSize(int a);

      void setBufferSize(int a);

      int get();
  };
}

#endif // _HTTP_SOCKETS_H
