#pragma once

#include <bloomail/base_clients/smtp_client.h>

namespace bloomail {

namespace Client {

class Gmail : public bloomail::BaseClient::Smtp {
public:
  Gmail(bool debug = false);
  ~Gmail();
};

} // namespace Client

} // namespace bloomail