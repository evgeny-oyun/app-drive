#include <iostream>
#include <string>
#include <map>
#include <gtest/gtest.h>

#include "../src/Http/Parser/Cookie.cpp"

using namespace Http::Parser;

class HttpParserCookieTest : public ::testing::Test
{
  protected:
  	void SetUp(){}
  	void TearDown(){}
};

TEST_F(HttpParserCookieTest, normalWithDelimiter)
{
  const std::string cookiesRaw = "normal=rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI;";
  std::map<std::string,std::string> cookiesList;

  ASSERT_NO_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList](const std::string *key, const std::string *value)
        {
            ASSERT_STREQ("normal", key->c_str());
            ASSERT_STREQ("rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI", value->c_str());

            cookiesList[*key] = *value;
        }
    )
  );

  ASSERT_EQ(cookiesList.size(), 1);
}


TEST_F(HttpParserCookieTest, normalWithoutDelimiter)
{
  std::string cookiesRaw = "normal=rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI";
  std::map<std::string,std::string> cookiesList;

  ASSERT_NO_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList](const std::string *key, const std::string *value)
        {
          ASSERT_STREQ("normal", key->c_str());
          ASSERT_STREQ("rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI", value->c_str());

          cookiesList[*key] = *value;
        }
    )
  );

  ASSERT_EQ(cookiesList.size(), 1);
}

TEST_F(HttpParserCookieTest, invalidCharsException)
{
  std::string cookiesRaw = "exception=A5QhSzblgcUyeSZMK; \n\n\r\t";
  std::map<std::string,std::string> cookiesList;

  ASSERT_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList](const std::string *key, const std::string *value){}
    ),
    Http::Exception
  );

  ASSERT_EQ(cookiesList.size(), 0);
}

TEST_F(HttpParserCookieTest, valueSpaceBefore)
{
  std::string cookiesRaw = "valueSpaceBefore=      t-ME0cmpwJHV9c8fffffffffa/A6QGGwZ-c1ymiDLAe; ";
  std::map<std::string,std::string> cookiesList;

  ASSERT_NO_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList](const std::string *key, const std::string *value)
        {
          ASSERT_STREQ("valueSpaceBefore", key->c_str());
          ASSERT_STREQ("t-ME0cmpwJHV9c8fffffffffa/A6QGGwZ-c1ymiDLAe", value->c_str());

          cookiesList[*key] = *value;
        }
    )
  );

  ASSERT_EQ(cookiesList.size(), 1);
}




TEST_F(HttpParserCookieTest, multiplePairs)
{
  std::string cookiesRaw  = "normal=rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI;";
              cookiesRaw += "valueSpaceBefore =      AjO7P9twI8fPXWeCY; ";
              cookiesRaw += "uid=A5QhSzblgcUyeSZMK;";
              cookiesRaw += "valueSpaceInner=t-ME0cmpwJHV9c8fffffffffa/A6QGGwZ-c1ymiDLAe; ";
              cookiesRaw += "       keySpaceBefore=7AxQ_-U72W3_Q7Dr/AtE7vWwDV2-dbROcT; ";
              cookiesRaw += "keySpaceAfter         = Q9PVQakLlyEYEOtutUnHL6QEcl8jwRU";

  std::map<std::string,std::string> cookiesList;

  ASSERT_NO_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList](const std::string *key, const std::string *value)
        {
          cookiesList[*key] = *value;
        }
    )
  );

  ASSERT_EQ(cookiesList.size(), 6);
}

TEST_F(HttpParserCookieTest, unexpectedPair)
{
  std::string cookiesRaw  = "normal=rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI;;hello=world;;;;;;;;;";

  std::map<std::string,std::string> cookiesList;
  size_t index = 0;

  ASSERT_NO_THROW(
    Cookie::parse(
        &cookiesRaw,
        [&cookiesList, &index](const std::string *key, const std::string *value)
        {
          if(index == 0)
          {
            ASSERT_STREQ("normal", key->c_str());
            ASSERT_STREQ("rwSfZIn9lm6D91AXp76sjmlbczU1CMmmI", value->c_str());
          }
          else if(index == 1)
          {
            ASSERT_STREQ("hello", key->c_str());
            ASSERT_STREQ("world", value->c_str());
          }

          cookiesList[*key] = *value;
          index++;
        }
    )
  );

  ASSERT_EQ(cookiesList.size(), 2);
}
