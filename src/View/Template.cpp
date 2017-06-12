#ifndef _VIEW_TEMPLATE_C
#define _VIEW_TEMPLATE_C

#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <map>
#include <sstream>
#include <vector>
#include <regex>
#include <sys/stat.h>

#include "../../deps/slre/slre.c"

#include "../System/Utils/Filesystem.cpp"
#include "../System/Utils/String.cpp"

#undef MAX_BRANCHES
#define MAX_BRANCHES 5

#undef MAX_BRACKETS
#define MAX_BRACKETS 5

#define TOKEN_VAR 0
#define TOKEN_IF 1
#define TOKEN_ENDIF 2
#define TOKEN_FOREACH 3
#define TOKEN_ENDFOREACH 4
#define TOKEN_INCLUDE 5
#define TOKEN_TEXT 6
#define TOKEN_INIT 7
#define TOKEN_KEY 8

namespace View{
  using namespace System::Utils::Filesystem;
  using namespace System::Utils;

  // slre regexp
  static const char *_tokens_reg [] = {
    "foreach \\$(\\S+)", // {foreach $array}
    "include \\x22(\\S+)\\x22", // {include "file/path.tpl"}
    "if\\s+\\$(\\S+)\\s+?==\\s+?\\x22(\\S+)\\x22", // {if $var == "value"}
    "if\\s+\\$(\\S+)", // {if $var}
    "if\\s+\\$(\\S+)\\s+?!=\\s+?\\x22(\\S+)\\x22", // {if $var == "value"}
  };

  //
  class Template{
  private:
    std::string                            str;
    std::map<std::string,std::string>     _variables;
    std::string                           _result;
  public:
    std::string compress();
    Template(const char *file)
    {
      str = file_get_contents(file);
    };

    Template(std::string file)
    {
      Template(file.c_str());
    };

    void assign(const std::string _k, const std::string _v)
    {
      _variables[_k] = _v;
    };

    void assign(const char *_k, const char *_v)
    {
      _variables[std::string(_k)] = std::string(_v);
    };

    void flush()
    {
      _variables.clear();
      _result.clear();
    };

    std::string render();
  };

  std::string Template::render()
  {
    size_t  _i      = 0;
    size_t  _max    = str.size();
    size_t  _length = 0;
    size_t  _index  = 0;
    //size_t  _fstart = 0;
    //size_t  _fend   = 0;

    uint8_t _state  = TOKEN_INIT;
    uint8_t _prev   = TOKEN_TEXT;

    bool    _valid  = true;

    std::string  _tmp;

    struct slre_cap caps[2];

    if(_result.size() > 0)
    {
      return _result;
    }

    for(_i = 0; _i < _max; _i++)
    {
      switch(_state)
      {
        case TOKEN_INIT:{
          if(str[_i] == '{' || _i == (_max - 1))
          {
            if(_valid)
            {
              _result += str.substr(_index, (_i == (_max - 1))?(_length + 1):_length);
            }

            _prev = TOKEN_TEXT;
            _state = TOKEN_KEY;
            _index = _i + 1;
            _length = 0;
            continue;
          }
          _length++;
        }
        break;
        case TOKEN_KEY:{
          if(str[_i] == '}')
          {
            if(_length == 3 && str[_index] == '/' && str[_index + 1] == 'i')
            {
              _valid = true;
              // nothing to do
              _prev = TOKEN_ENDIF;
            }
            else if(_length == 8 && str[_index] == '/' && str[_index + 1] == 'f')
            {
               if(_prev == TOKEN_FOREACH)
               {
                 //_fstart = 0;
                 //_fend = _index;
               }
              // not implemented yet
              _prev = TOKEN_ENDFOREACH;
            }
            else if(str[_index] == 'i' && str[_index + 1] == 'f')
            {
              if (slre_match(_tokens_reg[2], &str[_index], _length, caps, 2, 0) > 0)
              {
                _valid = false;

                if(_variables[std::string(caps[0].ptr, caps[0].len)] == std::string(caps[1].ptr, caps[1].len))
                {
                  _valid = true;
                }

                _prev = TOKEN_IF;
              }
              else if (slre_match(_tokens_reg[4], &str[_index], _length, caps, 2, 0) > 0)
              {
                _valid = false;

                if(_variables[std::string(caps[0].ptr, caps[0].len)] != std::string(caps[1].ptr, caps[1].len))
                {
                  _valid = true;
                }

                _prev = TOKEN_IF;
              }
              else if(slre_match(_tokens_reg[3], &str[_index], _length, caps, 2, 0) > 0)
              {
                _valid = false;
                _tmp = _variables[std::string(caps[0].ptr, caps[0].len)];

                if(_tmp.size() > 0)
                {
                  _valid = true;
                }

                _prev = TOKEN_IF;
              }
            }
            else if(str[_index] == 'i' && str[_index + 1] == 'n')
            {
              if (_valid && slre_match(_tokens_reg[1], &str[_index], _length, caps, 2, 0) > 0)
              {
                _tmp = str.substr(*caps[0].ptr, caps[0].len);

                if(_is_file_(_tmp.c_str()))
                {
                  _result += file_get_contents(_tmp.c_str());
                }

                _prev = TOKEN_INCLUDE;
              }
            }
            else if(str[_index] == 'f' && str[_index + 1] == 'o')
            {
              if (_valid && slre_match(_tokens_reg[0], &str[_index], _length, caps, 2, 0) > 0)
              {
                _tmp = str.substr(*caps[0].ptr, caps[0].len);
                _prev = TOKEN_FOREACH;

                if(_variables[_tmp].empty())
                {
                  _valid = false;
                  continue;
                }

                //_fstart = _index;
              }
            }
            else if(str[_index] == '$')
            {
              if(_valid)
              {
                _tmp = str.substr(_index + 1, _length - 1);
                _result += _variables[_tmp];
              }
              _prev = TOKEN_VAR;
            }

            _prev = TOKEN_TEXT;
            _index = _i + 1;
            _length = 0;
            _state = TOKEN_INIT;

            continue;
          }
          _length++;
        }
        break;
      }
    }

    return _result;
  }

  std::string Template::compress()
  {
    std::string       _compressed;

    _compressed = String::str_replace("\n", "", _result);
    _compressed = String::str_replace("\r", "", _compressed);
    _compressed = String::str_replace("  ", " ", _compressed);

    return _compressed;
  }
}

#endif // _VIEW_TEMPLATE_C
