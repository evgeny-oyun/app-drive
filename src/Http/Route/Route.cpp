#ifndef _HTTP_ROUTE_C
#define _HTTP_ROUTE_C

#include "Route.hpp"

namespace Http{

  namespace Route {

    using namespace std;

    Match Route::set(const string& u) {
      url = u;
      path_to_regex = regex(path_pattern);
      Match m(*this);
      return m;
    }

    string Match::get(const string& key) {
      if (pairs.count(key)) {
        return pairs.at(key);
      }
      return "";
    }

    bool Match::test(const string& tmpl) {

      pairs.clear();
      Path path;
      keys = 0;

      if (_Route->cache.count(tmpl)) {
        path = _Route->cache.at(tmpl);
      }

      else {
        //
        // get all the keys from the path.
        //
        sregex_token_iterator
          i(tmpl.begin(), tmpl.end(), _Route->path_to_regex),
          iend;

        while(i != iend) {
          string key = *i++;
          path.keys.push_back(key.erase(0, 1));
        }

        //
        // create a regex from the path.
        //
        auto exp = regex_replace(
          tmpl,
          _Route->path_to_regex,
          _Route->capture_pattern
        );
        path.re = regex("^" + exp + "$");
      }

      smatch sm_values;
      if (!regex_match(_Route->url, sm_values, path.re)) {
        return false;
      }

      if (sm_values.size() < 1) return true;

      for (size_t i = 0; i < sm_values.size() - 1; i++) {
        string key = path.keys[i];
        pairs.insert(pair<string, string>(key, sm_values[i + 1]));
        keys++;
      }
      return true;
    }

  } // namespace url
}

#endif // _HTTP_ROUTE_C
