#ifndef SYSTEM_DATABASE_DATABASE_H
#define SYSTEM_DATABASE_DATABASE_H

// dependecies
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <tuple>
#include "../Utils/String.cpp"
#include "../Utils/Filesystem.cpp"

namespace System{
  namespace Database{

    // types
    typedef std::vector<std::string> field_t;
    typedef std::unordered_map<std::string, std::string> row_t;
    typedef std::tuple<std::string, std::string, uint8_t> mapped_field_t;
    typedef std::vector<mapped_field_t> mapped_row_t;
    typedef void(callback_t)(size_t *index, row_t *row, void *user_data);

    // field types
    const uint8_t INTEGER = 1;
    const uint8_t STRING  = 2;
    const uint8_t TEXT    = 3;
    const uint8_t FLOAT   = 4;
    const uint8_t UNKNOWN = 5;

    class Database{
    protected:
      std::string _error;
    public:
      // properties

      // methods
      bool connect()
      {
        // implement
        return true;
      }

      bool query(const std::string query_str, callback_t handler, void *user_data)
      {
        // implement
        return true;
      }

      bool query(const std::string query_str, callback_t handler)
      {
        // implement
        return true;
      }

      bool query(const std::string)
      {
        // implement
        return true;
      }

      int getLastInsertID()
      {
        // implement
        return 0;
      }

      std::string error()
      {
        return _error;
      }
    };
  }
}

#endif // SYSTEM_DATABASE_DATABASE_H
