#include <bloomail/base_clients/smtp_client.h>

bloomail::BaseClient::Smtp::Smtp(const std::string &host, int port, bool debug)
    : tcp(host, port), tls(tcp), debug(debug) {
  this->recv();
  this->send("EHLO localhost");
  this->recv();
  return;
}

bloomail::BaseClient::Smtp::~Smtp() { return; }

void bloomail::BaseClient::Smtp::login(const std::string &username,
                                       const std::string &password) {
  this->username = brewtils::base64::encode(username);
  this->password = brewtils::base64::encode(password);
  this->startTls();
  this->authenticate();
  this->quit();
}

void bloomail::BaseClient::Smtp::send(const std::string &cmd) {
  if (this->tlsConnected) {
    this->tls.write(cmd + "\r\n");
  } else {
    this->tcp.write(cmd + "\r\n");
  }
}

std::string bloomail::BaseClient::Smtp::recv() {
  std::string response =
      this->tlsConnected ? this->tls.read() : this->tcp.read();
  if (this->debug) {
    logger::debug(response);
  }
  return response;
}

void bloomail::BaseClient::Smtp::startTls() {
  this->send("STARTTLS");
  this->recv();
  this->tls.connect();
  this->tlsConnected = true;
  this->send("EHLO localhost");
  this->recv();
}

void bloomail::BaseClient::Smtp::authenticate() {
  this->send("AUTH LOGIN");
  this->recv();
  this->send(this->username);
  this->recv();
  this->send(this->password);
  this->recv();
}

void bloomail::BaseClient::Smtp::quit() {
  this->send("QUIT");
  this->recv();
  this->tlsConnected = false;
}