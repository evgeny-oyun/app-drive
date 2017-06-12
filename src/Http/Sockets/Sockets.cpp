#ifndef _HTTP_SOCKET_C
#define _HTTP_SOCKET_C

#include "Sockets.hpp"


namespace Http {
  Socket::Socket(const char *address) {
    _socket = -1;
    _timeout = 60;
    _backlog = 1024;
    _buffer_size = 4096;
    _address = (char *) address;
  }

  bool Socket::setNonBlock(int _fd)
  {
      if ((fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL) | O_NONBLOCK)) == -1)
      {
          throw std::runtime_error(strerror(errno));
      }

      return true;
  }

  void Socket::setTimeout(int a) {
      _timeout = a;
  }

  void Socket::setBacklogSize(int a) {
      _backlog = a;
  }

  void Socket::setBufferSize(int a) {
      _buffer_size = a;
  }

  void Socket::_parse(const char *address) {
      char key[256], value[512];
      int i = 0, l = 0, k = 0, v = 0, f = 0, t = 0;

      memset(key, 0, 256);
      memset(value, 0, 512);

      if (address == NULL || (l = strlen(address)) <= 4) {
          throw std::runtime_error("Invalid socket listening address '" + std::string(address));
      }

      for (i = 0; i < l; i++) {
          if (isspace(address[i])) {
              continue;
          }

          if (address[i] == ':') {
              if (t == 0) {
                  key[k] = '\0';
              }
              t++;
          } else {
              if (t > 0 && isdigit(address[i])) {
                  value[v] = address[i];
                  v++;
              } else if (t == 0) {
                  if (address[i] == '/') {
                      f = 1;
                  }

                  key[k] = address[i];
                  k++;
              }
          }
      }

      key[k] = '\0';
      value[v] = '\0';

      if (f == 0 && v > 0) {
          _socket = _tcp(key, value);
      } else {
          _socket = _unix(key);
      }
  }

  int Socket::_unix(const char *path) {
      int len, socket_fd;
      struct sockaddr_un local;
      const unsigned int on = 1;
      struct timeval tmout;

      tmout.tv_sec = (time_t) _timeout;
      tmout.tv_usec = 0;

      if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
          throw strerror(errno);
      }

      memset(&local, 0, sizeof(struct sockaddr_un));

      local.sun_family = AF_UNIX;
      strncpy(local.sun_path, path, sizeof(local.sun_path));

      unlink(local.sun_path);

      len = strlen(local.sun_path) + sizeof(local.sun_family) + 1;

      if ((setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(const unsigned int))) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &_buffer_size, sizeof(int)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *) &tmout, sizeof(tmout)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *) &tmout, sizeof(tmout)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (bind(socket_fd, (struct sockaddr *) &local, len) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (listen(socket_fd, _backlog) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if(chmod(path, 0777) == -1)
      {
        throw std::runtime_error(strerror(errno));
      }

      return socket_fd;
  }

  int Socket::_tcp(const char *l_host, const char *l_port) {
      /* local variables */
      int socket_fd;
      const unsigned int on = 1;
      struct timeval tmout;
      struct addrinfo hints, *res;

      tmout.tv_sec = (time_t) _timeout;
      tmout.tv_usec = 0;

      memset(&hints, 0, sizeof hints);

      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      getaddrinfo(l_host, l_port, &hints, &res);

      /* init socket */
      if ((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
          throw std::runtime_error(strerror(errno));
      }


#if(defined __FreeBSD__)
      if((setsockopt(socket_fd, IPPROTO_TCP, TCP_NOPUSH, &on, sizeof(const unsigned int)))==-1)
      {
        throw std::runtime_error(strerror(errno));
      }
#endif

#if defined(__linux__)
      if((setsockopt(socket_fd, IPPROTO_TCP, TCP_CORK, &on, sizeof(const unsigned int)))==-1)
      {
        throw std::runtime_error(strerror(errno));
      }
#endif

      if ((setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(const unsigned int))) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &_buffer_size, sizeof(int)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tmout, sizeof(tmout)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (char *) &tmout, sizeof(tmout)) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      /* binding */
      if (bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
          throw std::runtime_error(strerror(errno));
      }

      /* listening */
      if (-1 == (listen(socket_fd, _backlog))) {
          throw std::runtime_error(strerror(errno));
      }

      freeaddrinfo(res);

      return socket_fd;
  }

  int Socket::get() {
      if (_socket == -1) {
          _parse(_address);
      }

      return _socket;
  }
}

#endif // _HTTP_SOCKET_C
