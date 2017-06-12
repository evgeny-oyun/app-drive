#ifndef _SYSTEM_SENDMAIL_C
#define _SYSTEM_SENDMAIL_C

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <curl/curl.h>
#include "Base64.cpp"

namespace Utils{
  struct upload_status{
    std::vector<std::string> payload_text;
    size_t position;
  };

  class Sendmail{
  protected:
    std::string _host = "smtp://example.com";
    std::string _user = "user";
    std::string _password = "";
  private:
    std::pair <std::string,std::string> from;
    std::pair <std::string,std::string> to;
    std::string subject;
    std::string message;
  public:
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
    {
      struct upload_status *upload_ctx = (struct upload_status *)userp;

      if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
      }

      if((upload_ctx->position + 1) <= upload_ctx->payload_text.size())
      {
        std::string line_str = upload_ctx->payload_text[upload_ctx->position];

        memcpy(ptr, line_str.c_str(), line_str.size());

        upload_ctx->position++;

        return line_str.size();
      }

      return 0;
    }

    void setFrom(const std::string _fa, const std::string _fb)
    {
      from.first = _fa;
      from.second = _fb;
    }

    void setTo(const std::string _ta, const std::string _tb)
    {
      to.first = _ta;
      to.second = _tb;
    }

    void setSubject(const std::string _s)
    {
      subject = _s;
    }

    void setMessage(const std::string _m)
    {
      message = _m;
    }

    bool send()
    {
      CURL *curl                    = NULL;
      CURLcode res                  = CURLE_OK;
      struct curl_slist *recipients = NULL;
      struct upload_status progress;
      bool result = false;

      curl = curl_easy_init();
      if(curl)
      {
        result = true;

        progress.position = 0;
        progress.payload_text.push_back("Content-Type: text/html; charset=UTF-8\r\n");
        progress.payload_text.push_back("MIME-Version: 1.0\r\n");
        progress.payload_text.push_back("To: " + to.second + "<" + to.first + ">\r\n");
        progress.payload_text.push_back("From: " + from.second + "<" + from.first + ">\r\n");
        progress.payload_text.push_back("Content-Type: text/html; charset=UTF-8\r\n");
        progress.payload_text.push_back("Subject: " + subject + "\r\n\r\n");
        progress.payload_text.push_back(message);

        curl_easy_setopt(curl, CURLOPT_URL, _host.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, _password.c_str());

        #ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

        #ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.first.c_str());

        recipients = curl_slist_append(recipients, to.first.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&progress);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
        {
          fprintf(
            stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res)
          );
          result = false;
        }

        /* Free the list of recipients */
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
      }

      return result;
    }

  };
}

#endif // _SYSTEM_SENDMAIL_C
