#include <iostream>

#include "src/http/http.h"
#include "src/socket.h"
int main() {
  Socket s = Socket(Address("www.baidu.com:80"));
  std::string raw = "";
  s.on<Open>([&]() {
    s.send(HTTP::RequestHeader(
               std::map<std::string, std::string>(
                   {{"Host", "www.baidu.com"}, {"Connection", "Close"}}),
               "GET", "/index.html", "")
               .toString());
  });
  s.on<Message>([&](const std::string& data) { raw += data; });
  s.on<Close>(
      [&]() { std::cout << HTTP::ResponseHeader(raw).content << std::endl; });
  s.start().join();
}