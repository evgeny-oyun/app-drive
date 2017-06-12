//
// Created by Evgeny Oyun on 07.01.17.
//

#ifndef HTTPD_HEADERS_H
#define HTTPD_HEADERS_H

#include <iostream>
#include <string>
#include <cstdio>
#include <unordered_map>
#include <sstream>

namespace Http
{
  using namespace std;

  class Headers
  {
    protected:
      unordered_map<string, string>  _headers;
      unordered_map<string, string>  _cookies;
      string _body;
    public:
      //
      void setHeader(const string &name, const int &value);
      void setHeader(const string &name, const string &value);
      string getHeader(const string name);

      //
      void setCookie(const string &name, const string &value);
      void setCookie(const string *name, const string *value);
      string getCookie(const string name);

      //
      void operator<<(const string &msg);
      void write(const string &msg);

      //
      const string body() const;
    };
}

#endif //HTTPD_HEADERS_H
