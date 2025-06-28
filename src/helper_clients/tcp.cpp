#include <bloomail/helper_clients/tcp.h>

bloomail::HelperClient::Tcp::Tcp(const std::string &host, int port) {
  addrinfo hints{}, *res;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res)) {
    logger::error(
        "Failed to resolve hostname " + host + ":" + std::to_string(port),
        "bloomail::HelperClient::Tcp::Tcp(const std::string &host, int port)");
  }

  this->sockfd =
      brewtils::sys::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (this->sockfd == -1 ||
      ::connect(this->sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    freeaddrinfo(res);
    logger::error(
        "Failed to connect to server at " + host + ":" + std::to_string(port),
        "bloomail::HelperClient::Tcp::Tcp(const std::string &host, int port)");
  }

  freeaddrinfo(res);
}

bloomail::HelperClient::Tcp::~Tcp() {
  if (this->sockfd != -1) {
    close(this->sockfd);
  }
}

int bloomail::HelperClient::Tcp::socket() const { return this->sockfd; }

std::string bloomail::HelperClient::Tcp::read() {
  char buff[4096];
  size_t n = brewtils::sys::recv(this->sockfd, buff, sizeof(buff) - 1, 0);
  if (n <= 0) {
    return {};
  }
  return std::string(buff, n);
}

void bloomail::HelperClient::Tcp::write(const std::string &data) {
  brewtils::sys::send(this->sockfd, data.c_str(), data.size(), 0);
}