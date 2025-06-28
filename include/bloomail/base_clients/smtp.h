#pragma once

#include <set>

#include <brewtils/base64.h>

#include <bloomail/base_clients/i_base_client.h>
#include <bloomail/helper_clients/tls.h>

namespace bloomail {

namespace BaseClient {

class Smtp : public IBaseClient {
private:
  bool tlsConnected;

  bloomail::HelperClient::Tcp tcp;
  bloomail::HelperClient::Tls tls;

  void send(const std::string &cmd) override;
  std::string recv() override;
  void startTls();
  void authenticate() override;
  void quit() override;

public:
  Smtp(const std::string &host, int port, bool debug = false);
  ~Smtp();

  void login(const std::string &username, const std::string &password);
  void sendEmail() override;
};

} // namespace BaseClient

} // namespace bloomail