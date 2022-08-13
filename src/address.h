/*
  This project was under the MIT license.
  Copyright (c) 2022 FurryR
*/
#ifndef LIBNETWORK_ADDRESS_H
#define LIBNETWORK_ADDRESS_H
#ifdef _WIN32
#include <winsock.h>
typedef uint32_t in_addr_t;
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <stdexcept>
#include <string>
typedef class Address {
  static std::string ntoa(const std::string& host) {
    hostent* ptr;
    sockaddr_in addr{};
    if ((ptr = gethostbyname(host.c_str())) == nullptr) {
      switch (h_errno) {
        case HOST_NOT_FOUND:
          throw std::runtime_error("Host not found");
        case NO_DATA:
          throw std::runtime_error("No data record found");
        case NO_RECOVERY:
          throw std::runtime_error("Nonrecoverable error");
        case TRY_AGAIN:
          throw std::runtime_error("Temporary error");
      }
    }
    addr.sin_addr.s_addr = *(in_addr_t*)(ptr->h_addr_list[0]);
    return inet_ntoa(addr.sin_addr);
  }

 public:
  std::string host;
  uint16_t port;
  Address() {}
  Address(const std::string& host, uint16_t port)
      : host(ntoa(host)), port(port) {}
  Address(const std::string& url) {
    size_t delim = url.find(':');
    if (delim == std::string::npos) throw std::runtime_error("Invalid url");
    host = ntoa(url.substr(0, delim));
    port = std::stoi(url.substr(delim + 1));
  }
} Address;
#endif