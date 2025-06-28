#pragma once

#include <brewtils/base64.h>

#include <bloomail/helper_clients/tls_client.h>

namespace bloomail {

namespace BaseClient {

class Smtp {
private:
  bool debug;
  bool tlsConnected;
  std::string username;
  std::string password;

  bloomail::HelperClient::Tcp tcp;
  bloomail::HelperClient::Tls tls;

  void send(const std::string &cmd);
  std::string recv();
  void startTls();
  void authenticate();
  void quit();

public:
  Smtp(const std::string &host, int port, bool debug = false);
  ~Smtp();

  void login(const std::string &username, const std::string &password);
};

} // namespace BaseClient

} // namespace bloomail