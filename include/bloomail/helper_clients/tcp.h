#pragma once

#include <netdb.h>

#include <brewtils/sys.h>

namespace bloomail {

namespace HelperClient {

class Tcp {
private:
  int sockfd;

public:
  Tcp(const std::string &host, int port);
  ~Tcp();

  int socket() const;
  std::string read();
  void write(const std::string &data);
};

} // namespace HelperClient

} // namespace bloomail