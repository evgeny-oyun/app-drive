#ifndef SYSTEM_DATABASE_MYSQL_C
#define SYSTEM_DATABASE_MYSQL_C

#include "Database.hpp"

//
#include <my_global.h>
#include <mysql.h>

namespace System{
  namespace Database{
    class MySQL: public Database{
    private:
      //
      MYSQL   *resource;

      //
      bool fetch(callback_t handler, void *user_data)
      {
        MYSQL_RES *result = mysql_store_result(resource);

        if(result == NULL)
        {
          return false;
        }

        int rows_count   = mysql_num_rows(result);
        int fields_count = mysql_num_fields(result);

        if(rows_count > 0)
        {
          MYSQL_ROW    row;
          MYSQL_FIELD *field;

          field_t keys;
          int     f = 0;
          size_t  r = 0;

          while((field = mysql_fetch_field(result)) != NULL)
          {
            keys.push_back(std::string(field->name));

            f++;
          }

          while ((row = mysql_fetch_row(result)))
          {
            row_t _row;

            for(f = 0; f < fields_count; f++)
            {
              _row[keys[f]] = std::string(row[f] ? row[f] : "");
            }

            //
            handler(&r, &_row, user_data);

            r++;
          }

          mysql_free_result(result);

          return true;
        }

        return false;
      }
    public:
      ~MySQL()
      {
        mysql_close(resource);
      };

      bool connect(const std::string host, const std::string user, const std::string password, const std::string database)
      {
        if((resource = mysql_init(NULL)) == NULL)
        {
          _error = "Unable to initialize MySQL instance";
          return false;
        }

        resource = mysql_real_connect(
          resource,
          host.c_str(),
          user.c_str(),
          password.c_str(),
          database.c_str(),
          0,
          NULL,
          CLIENT_IGNORE_SIGPIPE | CLIENT_MULTI_STATEMENTS
        );

        if(resource == NULL)
        {
          _error  = "Unable to connect to MySQL: ";
          _error += std::string(mysql_error(resource));
          return false;
        }

        if(mysql_query(resource, "SET NAMES 'UTF8'"))
        {
          _error  = "Unable to set connection transfer encoding to UTF-8: ";
          _error += std::string(mysql_error(resource));
          return false;
        }

        return true;
      };

      bool query(const std::string query_str, callback_t handler, void *user_data)
      {
        if(query(query_str) == true)
        {
          return fetch(handler, user_data);
        }

        return false;
      }

      bool query(const std::string query_str, callback_t handler)
      {
        if(query(query_str) == true)
        {
          return fetch(handler, NULL);
        }

        return false;
      }

      bool query(const std::string query_str)
      {
        if (mysql_query(resource, query_str.c_str()) == 0)
        {
          _error = std::string(mysql_error(resource));
          return false;
        }

        return true;
      }

      bool insert(const std::string table, const row_t _row)
      {
        if(_row.size() > 0)
        {
            std::string _query_str;

            _query_str = "INSERT INTO `" +table+ "`(";

            size_t _i = 0;
            size_t _max = (_row.size() - 1);

            std::string _keys;
            std::string _values;

            for(auto &field: _row)
            {
                _keys += "`" +field.first+ "`";
                _values += "'" + System::Utils::String::replace("'", "\\'",field.second) + "'";

                if(_i < _max)
                {
                    _keys += ", ";
                    _values += ", ";
                }

                _i++;
            }

            _query_str += _keys + ") VALUES(";
            _query_str += _values + ")";

            std::cout << _query_str << std::endl;

            return query(_query_str);
        }

        return false;
      }

      int getLastInsertID()
      {
        return mysql_insert_id(resource);
      }
    };
  }
}

#endif //SYSTEM_DATABASE_MYSQL_C
