#ifndef _SYSTEM_EVENTS_C
#define _SYSTEM_EVENTS_C

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

namespace System
{
  namespace Events
  {
    using namespace std;

    /*
     * Base system event
     */
    class BaseEvent
    {
    public:
      virtual void handler(){}
    };

    /*
     * System events pool
     */
    class Events
    {
    private:
      /* events list */
      unordered_map<string,vector<BaseEvent*>> pool;

      /* reduce memory access errors */
      bool is_valid(string &key, BaseEvent *value, bool validate_list)
      {
        if(key.size() == 0 || value == NULL)
        {
          return false;
        }

        if(validate_list == true)
        {
          auto list = pool[key];

          if(list.size() > 0)
          {
            return true;
          }
        }
        else
        {
          return true;
        }

        return false;
      }
    public:
      /*
       * Bind an event specified by key string
       */
      void attach(string &key, BaseEvent *value)
      {
        if(is_valid(key,value, false))
        {
          pool[key].push_back(value);
        }
      }

      /*
       * Unbind an event
       */
      void detach(string &key, BaseEvent *value)
      {
        if(is_valid(key, value, true))
        {
          size_t i = 0;

          for(auto &ev: pool[key])
          {
            if(ev == value)
            {
              pool[key].erase(pool[key].begin() + i);
            }

            i++;
          }
        }
      }

      /*
       * Unbind all events by key
       */
      void detach(string &key)
      {
        auto list = pool[key];

        if(list.size() > 0)
        {
          pool[key].clear();
        }
      }

      /*
       * Triggering an event 
       */
      void dispatch(string &key)
      {
        auto list = pool[key];

        if(list.size() > 0)
        {
          for(auto &ev: list)
          {
            ev->handler();
          }
        }
      }
    };
  }
}

#endif // _SYSTEM_EVENTS_C
