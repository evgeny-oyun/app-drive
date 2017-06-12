#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#ifndef SYSTEM_UTILS_FILESYSTEM_C
#define SYSTEM_UTILS_FILESYSTEM_C

namespace System{
  namespace Utils{
    namespace Filesystem{

        bool is_file(const char* f)
        {
          struct stat st;
          lstat(f, &st);
          return S_ISREG(st.st_mode)?true:false;
        }

        bool is_file(std::string &str)
        {
          return is_file(str.c_str());
        }

        // read whole file into string using stream
        std::string file_get_contents(const char *file)
        {
          std::string str;

          if(is_file(file))
          {
            std::ifstream t(file);

              t.seekg(0, std::ios::end);

              str.reserve(t.tellg());

              t.seekg(0, std::ios::beg);

              str.assign(
                (std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>()
              );
          }

          return str;
        }

        std::string file_get_contents(std::string &str)
        {
          return file_get_contents(str.c_str());
        }
    }
  }
}

#endif // SYSTEM_UTILS_FILESYSTEM_C
