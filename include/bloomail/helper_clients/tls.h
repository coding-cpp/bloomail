#pragma once

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <bloomail/helper_clients/tcp.h>

namespace bloomail {

namespace HelperClient {

class Tls {
private:
  SSL_CTX *ctx;
  SSL *ssl;
  bloomail::HelperClient::Tcp tcp;

public:
  explicit Tls(bloomail::HelperClient::Tcp &client);
  ~Tls();

  void connect();
  void write(const std::string &data);
  std::string read();
};

} // namespace HelperClient

} // namespace bloomail