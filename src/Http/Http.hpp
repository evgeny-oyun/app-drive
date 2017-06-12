#ifndef _HTTP_H
#define _HTTP_H

#include <iostream>
#include <string>
#include <exception>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define HTTPD_BUFFER_SIZE   4096
#define HTTPD_TIMEOUT       60
#define HTTPD_BACKLOG_SIZE  512

namespace Http
{
  using namespace std;

  const uint8_t CONNECTION_STATE_REQUEST_READ         = 1;
  const uint8_t CONNECTION_STATE_REQUEST_PROCESS      = 2;
  const uint8_t CONNECTION_STATE_REQUEST_ERROR        = 3;
  const uint8_t CONNECTION_STATE_RESPONSE_HEADERS     = 4;
  const uint8_t CONNECTION_STATE_RESPONSE_BODY        = 5;
  const uint8_t CONNECTION_STATE_RESPONSE_COMPLETED   = 6;

  const unordered_map<int, string> RESPONSE_CODES = {
    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Payload Too Large"},
    {414, "URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {421, "Misdirected Request"},
    {422, "Unprocessable Entity"},
    {423, "Locked"},
    {424, "Failed Dependency"},
    {426, "Upgrade Required"},
    {428, "Precondition Required"},
    {429, "Too Many Requests"},
    {431, "Request Header Fields Too Large"},
    {451, "Unavailable For Legal Reasons"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"},
    {506, "Variant Also Negotiates"},
    {507, "Insufficient Storage"},
    {508, "Loop Detected"},
    {510, "Not Extended"},
    {511, "Network Authentication Required"}
  };
}

#include "Headers/Headers.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Connection/Connection.hpp"
#include "Server/Server.hpp"

#endif // _HTTP_H
