#ifndef _SYSTEM_ENCRYPT_C
#define _SYSTEM_ENCRYPT_C

#include <iostream>
#include <string>
#include "Base64.cpp"

namespace System{
    namespace Utils{
        using namespace std;
        class Encrypt{
        public:
            static const int default_key = ((2 ^ 8) | (3 << 4)) ^ 5;

            static std::string encrypt(std::string &str)
            {
                size_t i;

                for(i = 0; i < str.size(); ++i)
                {
                    str[i] = str[i] - Encrypt::default_key;
                }

                return base64_encode((const unsigned char*)str.c_str(), (unsigned int)str.size());
            }

            static std::string decrypt(std::string &str_encoded)
            {
                size_t i;

                std::string str = base64_decode(str_encoded);

                for(i = 0; i < str.size(); ++i)
                {
                    str[i] = str[i] + Encrypt::default_key;
                }

                return str;
            }
        };
    }
}
#endif // _SYSTEM_ENCRYPT_C
