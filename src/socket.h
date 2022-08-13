/*
  This project was under the MIT license.
  Copyright (c) 2022 FurryR
*/
#ifndef LIBNETWORK_SOCKET_H
#define LIBNETWORK_SOCKET_H

#include <unistd.h>

#include <functional>
#include <string>
#include <thread>
#ifdef _WIN32
#include <winsock.h>
class __WinSockInit {
  WSADATA _wsa;

 public:
  __WinSockInit() { WSAStartup(MAKEWORD(2, 0), &_wsa); }
  ~__WinSockInit() { WSACleanup(); }
} __WinSockInit;
#endif

#include "address.h"
typedef enum EventType {
  Open = 0,
  Message = 1,
  Close = 2,
  Error = 3
} EventType;
typedef struct Socket {
 public:
  /**
   * @brief Construct a new Socket object
   */
  Socket() {}
  /**
   * @brief Construct a new Socket object
   * @param addr The address of the socket
   */
  Socket(const Address& addr) : addr(addr), fd(-1) {}
  /**
   * @brief Destroy the Socket object
   */
  ~Socket() { close(); }
  template <EventType>
  /**
   * @brief Register a callback function for the specified event type
   * @param fn The callback function
   */
  void on(const std::function<void()>& fn) noexcept;
  template <EventType>
  /**
   * @brief Register a callback function for the specified event type
   * @param fn The callback function
   */
  void on(const std::function<void(const std::string&)>& fn) noexcept;
  /**
   * @brief Apply the request
   * @return a event thread
   */
  std::thread start() { return std::thread(&Socket::run, this); }
  /**
   * @brief Send data to the socket
   * @param data The data to send
   * @return ssize_t the size of the sent data
   */
  ssize_t send(const std::string& data) const {
    ssize_t result;
    if (fd == -1) throw std::runtime_error("socket not open");
    if ((result = ::send(fd, data.c_str(), data.length(), 0)) == -1)
      throw std::runtime_error("send failed");
    return result;
  }
  /**
   * @brief Close the socket
   */
  void close() noexcept {
    if (fd == -1) return;
    ::close(fd);
    fd = -1;
  }

 private:
  std::function<void(const std::string&)> onmessage;
  std::function<void()> onclose;
  std::function<void()> onerror;
  std::function<void()> onopen;
  Address addr;
  int fd;
  void run() {
    char buf[1024];
    ssize_t s;
    sockaddr_in sin{};
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      throw std::runtime_error("socket failed");
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(addr.host.c_str());
    sin.sin_port = htons(addr.port);
    if (connect(fd, (sockaddr*)&sin, sizeof(sin)) < 0) {
      close();
      if (onerror) return onerror();
    }
    if (onopen) onopen();
    while ((s = recv(fd, buf, 1024, 0)) > 0)
      if (onmessage) onmessage(std::string(buf, 0, s));
    if (onclose) onclose();
    close();
  }
} Socket;
template <>
/**
 * @brief Register a callback function for the specified event type
 * @param fn The callback function
 */
void Socket::on<Error>(const std::function<void()>& fn) noexcept {
  onerror = fn;
}
template <>
/**
 * @brief Register a callback function for the specified event type
 * @param fn The callback function
 */
void Socket::on<Message>(
    const std::function<void(const std::string&)>& fn) noexcept {
  onmessage = fn;
}
template <>
/**
 * @brief Register a callback function for the specified event type
 * @param fn The callback function
 */
void Socket::on<Close>(const std::function<void()>& fn) noexcept {
  onclose = fn;
}
template <>
/**
 * @brief Register a callback function for the specified event type
 * @param fn The callback function
 */
void Socket::on<Open>(const std::function<void()>& fn) noexcept {
  onopen = fn;
}
#endif
