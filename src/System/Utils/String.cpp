#ifndef _SYSTEM_UTILS_STRING_C
#define _SYSTEM_UTILS_STRING_C

#include <iostream>
#include <string>

namespace System
{
    namespace Utils
    {

      /* used pgen library code */
      char *random_string(unsigned int length)
      {
        unsigned int  i;
        unsigned int  nrand;
        char         *pw = NULL;

        /*
         * Values of small latin symbols.
        */
        char s_latin[] = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
        };

        /*
         * Values of big latin symbols.
        */
        char s_glatin[] = {
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
        };

        /*
         * Values of special ANSI symbols.
        */
        char s_esc[] = {
            '0','1','2','3','4','5','6','7','8','9'
        };

        /*
         * Allocating memory for new password array.
        */
        pw = (char *) malloc (length * sizeof(char)+1);

        if(pw!=NULL)
        {
          for (i = 0; i < length; i++)
          {
            nrand = rand() % 100;

            if (nrand > 50)
            {
              pw[i] = s_latin[rand() % sizeof(s_latin)];
            }
            else if (nrand > 20 && nrand <= 50)
            {
              pw[i] = s_glatin[rand() % sizeof(s_glatin)];
            }
            else{
              pw[i] = s_esc[rand() % sizeof(s_esc)];
            }
          }

          pw[length]='\0';
        }

        return pw;
      }


      class String{
      public:
        static std::string replace(const std::string& search,
                                  const std::string& replace,
                                  const std::string& subject)
          {
            std::string str = subject;
            size_t pos = 0;
            while((pos = str.find(search, pos)) != std::string::npos)
            {
              str.replace(pos, search.length(), replace);
              pos += replace.length();
            }
            return str;
          };

          static std::string random(size_t length)
          {
            char *tmp = random_string(length);
            std::string result(tmp);
            free(tmp);

            return result;
          };
      };
    }
}

#endif // _SYSTEM_UTILS_STRING_C
