
#ifndef SYSTEM_UTILS_CACHE_C
#define SYSTEM_UTILS_CACHE_C

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../../../deps/md5/md5.cpp"
#include "Filesystem.cpp"

namespace System{
  namespace Utils{
    using namespace System::Utils::Filesystem;

    class Cache{
    private:
      std::vector<std::string> _tokens; // temporary
      std::string _hash;
      std::string _prefix = "cache_";
      std::string _path = "/tmp/";
      bool _existance = false;
      std::string _result;
    public:
      void addToken(const std::string token)
      {
        _tokens.push_back(token);
      }

      void init()
      {
        for(auto &t : _tokens)
        {
          _hash += t + "@";
        }

        char *hmd5 = md5((unsigned char*)_hash.c_str());

        _hash = std::string(hmd5);

        _tokens.clear();

        _path = _path + _prefix + _hash;

        _existance = is_file(_path.c_str());

        if(_existance)
        {
          _result = file_get_contents(_path.c_str());
        }

        free(hmd5);
      }

      bool exists()
      {
        return _existance;
      }

      std::string get()
      {
        return _result;
      }

      void save(std::string &str)
      {
        std::fstream _stream;

        _stream = std::fstream(
          _path,
          std::ios::out | std::ios::binary
        );

        _stream.write(str.c_str(), str.size());

        _stream.close();
      }
    };
  }
}

#endif // SYSTEM_UTILS_CACHE_C
