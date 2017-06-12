#include "src/Http/Http.cpp"

using namespace Http;

int main(int argc, char const *argv[]) {
  Server hello;
  hello.bind(
    "/tmp/api.sock",
    [](auto req, auto res)
    {
      res->setStatus(200);
      res->setHeader("Content-Type", "text/html;charset=UTF-8");

      res->write("Hello, world!<br>");
      res->write(req->dump());

      return true;
    }
  );

  Server::loop();

  return 0;
}
