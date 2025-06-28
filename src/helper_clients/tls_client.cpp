#include <bloomail/helper_clients/tls_client.h>

bloomail::HelperClient::Tls::Tls(bloomail::HelperClient::Tcp &client)
    : tcp(client) {
  SSL_library_init();
  SSL_load_error_strings();
  this->ctx = SSL_CTX_new(TLS_client_method());
  if (!this->ctx) {
    logger::error("Failed to craete SSL_CTX",
                  "bloomail::HelperClient::Tls::Tls(bloomail::HelperClient::"
                  "Tcp &client)");
  }
}

bloomail::HelperClient::Tls::~Tls() {
  if (this->ssl) {
    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
  }
  if (this->ctx) {
    SSL_CTX_free(this->ctx);
  }
}

void bloomail::HelperClient::Tls::connect() {
  this->ssl = SSL_new(ctx);
  SSL_set_fd(this->ssl, this->tcp.socket());
  if (SSL_connect(this->ssl) <= 0) {
    logger::error("TLS Handshake failed",
                  "void bloomail::HelperClient::Tls::connect()");
  }
}

void bloomail::HelperClient::Tls::write(const std::string &data) {
  if (!this->ssl) {
    logger::error(
        "SSL not initialized",
        "void bloomail::HelperClient::Tls::write(const std::string &data)");
  }
  SSL_write(this->ssl, data.c_str(), data.size());
}

std::string bloomail::HelperClient::Tls::read() {
  char buff[4096];
  int len = SSL_read(ssl, buff, sizeof(buff) - 1);
  if (len <= 0) {
    return {};
  }
  return std::string(buff, len);
}