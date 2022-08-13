#ifndef LIBNETWORK_HTTP_H
#define LIBNETWORK_HTTP_H
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
namespace HTTP {
// Internal Function,do not use.
const std::vector<std::string> _split(const std::string &str,
                                      const char delim) noexcept {
  std::vector<std::string> s;
  std::string temp;
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == delim) {
      s.push_back(temp);
      temp = "";
    } else
      temp += str[i];
  }
  if (temp != "") s.push_back(temp);
  return s;
}

// Request Header class.
typedef struct RequestHeader {
  // settings.
  std::map<std::string, std::string> conf;
  // Action(POST,GET,...)
  std::string action;
  // path which is being accessed.
  std::string path;
  // Request's content.
  std::string content;
  // convert to string.
  const std::string toString() const noexcept {
    std::string ret;
    ret = action + " ";
    ret += path + " HTTP/1.0\r\n";
    for (std::map<std::string, std::string>::const_iterator i = conf.begin();
         i != conf.end(); i++) {
      ret += i->first + ": " + i->second + "\r\n";
    }
    ret += "\r\n";
    ret += content;
    return ret;
  }
  RequestHeader() {}
  // construct by value.
  RequestHeader(const std::map<std::string, std::string> &_conf,
                const std::string &_action, const std::string &_path,
                const std::string &_content) {
    conf = _conf;
    action = _action;
    path = _path;
    content = _content;
  }
  // construct by raw header.
  RequestHeader(const std::string &str) {
    std::vector<std::string> s = _split(str, '\n');
    if (s.size() < 1) throw std::runtime_error("Invalid Header");
    std::vector<std::string> w = _split(s[0].substr(0, s[0].length() - 1), ' ');
    if (w.size() != 3) throw std::runtime_error("Invalid Header");
    action = w[0];
    path = w[1];
    for (size_t i = 1, a = 0; i < s.size(); i++) {
      if (s[i] == "\r") {
        a = 1;
        continue;
      }
      if (a == 1) {
        content += s[i] + '\n';
      } else {
        w = _split(s[i], ':');
        if (w.size() < 2) throw std::runtime_error("Invalid Header");
        conf[w[0]] = w[1].substr(1, w[1].length() - 2);
      }
    }
    content = content.substr(0, content.length() - 1);
  }
} RequestHeader;
// Responce Header class.
typedef struct ResponseHeader {
  // settings.
  std::map<std::string, std::string> conf;
  // HTTP status(200 OK).
  std::string status;
  // Returned content.
  std::string content;
  // convert to string.
  const std::string toString() const {
    std::string ret = "HTTP/1.1 " + status + "\r\n";
    for (std::map<std::string, std::string>::const_iterator i = conf.begin();
         i != conf.end(); i++) {
      ret += i->first + ": " + i->second + "\r\n";
    }
    ret += "\r\n";
    ret += content;
    return ret;
  }
  ResponseHeader() {}
  // construct by value.
  ResponseHeader(const std::map<std::string, std::string> &_conf,
                 const std::string &_status, const std::string &_content) {
    conf = _conf;
    status = _status;
    content = _content;
  }
  // construct by raw header.
  ResponseHeader(const std::string &str) {
    std::vector<std::string> s = _split(str, '\n');
    if (s.size() < 1) throw std::runtime_error("Invalid Header");
    std::vector<std::string> w = _split(s[0].substr(0, s[0].length() - 1), ' ');
    if (w.size() < 3) throw std::runtime_error("Invalid Header");
    status = s[0].substr(w[0].size() + 1);
    for (size_t i = 1, a = 0; i < s.size(); i++) {
      if (s[i] == "\r") {
        a = 1;
        continue;
      }
      if (a == 1) {
        content += s[i] + '\n';
      } else {
        w = _split(s[i], ':');
        if (w.size() < 2) throw std::runtime_error("Invalid Header");
        conf[w[0]] = w[1].substr(1, w[1].length() - 2);
      }
    }
    if (content != "") content = content.substr(0, content.length() - 1);
  }
} ResponseHeader;
};  // namespace HTTP
#endif