#ifndef SYSTEM_DATABASE_SQLITE_C
#define SYSTEM_DATABASE_SQLITE_C

#include "Database.hpp"

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

namespace System{
  namespace Database{
    /*
     * Currently all queries will block.
     * @TODO: Make async non-block i/o
     */
    class SQLite{
    private:
      // properties
      sqlite3         *db;
      sqlite3_stmt    *stmt;
      row_t            row;
      int              result;

      // get index of :parameter wrapper
      int bind_index(const std::string field_name)
      {
        return sqlite3_bind_parameter_index(stmt, std::string(":" + field_name).c_str());
      }
    public:
      ~SQLite()
      {
        sqlite3_close(db);
      };
      
      void close()
      {
        sqlite3_close(db);
      }

      /*
       * Attach a database file
       */
      void open(const std::string file_path)
      {
        if((result = sqlite3_open(file_path.c_str(), &db)) != SQLITE_OK)
        {
          throw std::runtime_error(sqlite3_errstr(result));
        }
      };

      /*
       * A query string may contain parameters
       * For example, "SELECT id FROM `table` WHERE `id` = :id"
       */
      bool query(const std::string &query_str)
      {
        row.clear();
        sqlite3_finalize(stmt);
        stmt = NULL;

        return (result = sqlite3_prepare_v2(db, query_str.c_str(), query_str.size(), &stmt, NULL)) == SQLITE_OK;
      }

      bool exec()
      {
        if(stmt == NULL)
        {
            return false;
        }

        result = sqlite3_step(stmt);

        if(result == SQLITE_OK || result == SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            stmt = NULL;

            return true;
        }

        return false;
      }

    /*
     * Fetch a result
     */
    row_t *fetch()
    {
      result = sqlite3_step(stmt);

      if(result == SQLITE_ROW)
      {
        int columns_count = sqlite3_column_count(stmt);

        // reset row state
        row.clear();

        for(int i = 0; i < columns_count; i++)
        {
          switch (sqlite3_column_type(stmt, i))
          {
            case SQLITE_INTEGER:{
              row.insert(
                std::make_pair(
                  std::string(sqlite3_column_name(stmt, i)),
                  std::to_string(sqlite3_column_int(stmt, i))
                )
              );
            }
            break;
            case SQLITE_TEXT:{
              row.insert(
                std::make_pair(
                  std::string(sqlite3_column_name(stmt, i)),
                  std::string((const char*)sqlite3_column_text(stmt, i))
                )
              );
            }
            break;
            case SQLITE_FLOAT:{
              row.insert(
                std::make_pair(
                  std::string(sqlite3_column_name(stmt, i)),
                  std::to_string(sqlite3_column_double(stmt, i))
                )
              );
            }
            break;
            case SQLITE_NULL:{
              row.insert(
                std::make_pair(
                  std::string(sqlite3_column_name(stmt, i)),
                  std::string("")
                )
              );
            }
            break;
            case SQLITE_BLOB:{
              row.insert(
                std::make_pair(
                  std::string(sqlite3_column_name(stmt, i)),
                  std::string((const char*)sqlite3_column_blob(stmt, i))
                )
              );
            }
            break;
          }
        }

        return &row;
      }
      else
      {
          row.clear();
          sqlite3_finalize(stmt);
          stmt = NULL;
      }

      return NULL;
    }

      bool bind(const std::string field_name, const int field_value)
      {
        return sqlite3_bind_int(stmt, bind_index(field_name), field_value) == SQLITE_OK;
      }

      bool bind(const std::string field_name, const std::string field_value)
      {
        return sqlite3_bind_text(stmt, bind_index(field_name), field_value.c_str(), field_value.size(), NULL) == SQLITE_OK;
      }

      bool bind(const std::string field_name, const double field_value)
      {
        return sqlite3_bind_double(stmt, bind_index(field_name), field_value) == SQLITE_OK;
      }

      /*
       * A wrapper of raw INSERT query
       */
      bool insert(const std::string &table, const mapped_row_t &row)
      {
        std::string   query_str   = "INSERT INTO `" + table + "` (";
        std::string   keys, values;

        size_t        i = 0,
                      max = (row.size() - 1);

        for(auto &field: row)
        {
          std::string   field_name, field_value;
          uint8_t       field_type;

          std::tie(field_name, field_value, field_type) = field;

          keys    += "`" + field_name + "`";
          values  += "?";

          if(i < max)
          {
              keys += ", ";
              values += ", ";
          }

          i++;
        }

        query_str += keys + ") VALUES(";
        query_str += values + ")";


        sqlite3_stmt   *stmt;
        int            result;

        //
        result = sqlite3_prepare_v2(
          db,
          query_str.c_str(),
          -1,
          &stmt,
          NULL
        );

      	if(result == SQLITE_OK)
        {
          i = 1;
          for(auto &field: row)
          {
            if(std::get<2>(field) == INTEGER)
            {
              sqlite3_bind_int(
                stmt,
                i,
                atoi(std::get<1>(field).c_str())
              );
            }
            else if(std::get<2>(field) == STRING)
            {
              sqlite3_bind_text(
                stmt,
                i,
                std::get<1>(field).c_str(),
                std::get<1>(field).size(),
                0
              );
            }
            else if(std::get<2>(field) == FLOAT)
            {
              sqlite3_bind_double(
                stmt,
                i,
                atof(std::get<1>(field).c_str())
              );
            }

            i++;
          }

          // @TODO: Check for done
          sqlite3_step(stmt);
          sqlite3_finalize(stmt);

          return true;
        }

        return false;
      }

      /*
       * Update a record in the table
       */
      bool update(const std::string &table, const mapped_row_t &row, int id)
      {
        std::string   query_str   = "UPDATE `" + table + "` SET ";

        size_t        i = 0,
                      max = (row.size() - 1);

        for(auto &field: row)
        {
          std::string   field_name, field_value;
          uint8_t       field_type;

          std::tie(field_name, field_value, field_type) = field;

          query_str += "`" + field_name + "` = ?";

          if(i < max)
          {
              query_str += ", ";
          }

          i++;
        }

        query_str += " WHERE id=" + std::to_string(id);

        sqlite3_stmt   *stmt;
        int            result;

        //
        result = sqlite3_prepare_v2(
          db,
          query_str.c_str(),
          -1,
          &stmt,
          NULL
        );

      	if(result == SQLITE_OK)
        {
          i = 1;
          for(auto &field: row)
          {
            if(std::get<2>(field) == INTEGER)
            {
              sqlite3_bind_int(
                stmt,
                i,
                atoi(std::get<1>(field).c_str())
              );
            }
            else if(std::get<2>(field) == STRING)
            {
              sqlite3_bind_text(
                stmt,
                i,
                std::get<1>(field).c_str(),
                std::get<1>(field).size(),
                0
              );
            }
            else if(std::get<2>(field) == FLOAT)
            {
              sqlite3_bind_double(
                stmt,
                i,
                atof(std::get<1>(field).c_str())
              );
            }

            i++;
          }

          // @TODO: Check for done
          sqlite3_step(stmt);
          sqlite3_finalize(stmt);

          return true;
        }

        return false;
      }

      int getLastInsertID()
      {
        return sqlite3_last_insert_rowid(db);
      }

      std::string error()
      {
        return std::string(sqlite3_errstr(result));
      }

    };
  }
}

#endif //SYSTEM_DATABASE_SQLITE_C
