#include <bloomail/base_clients/smtp.h>

bloomail::BaseClient::Smtp::Smtp(const std::string &host, int port, bool debug)
    : tcp(host, port), tls(tcp), bloomail::BaseClient::IBaseClient(debug),
      tlsConnected(false) {
  this->recv();
  this->send("EHLO localhost");
  this->recv();
  return;
}

bloomail::BaseClient::Smtp::~Smtp() {
  this->quit();
  return;
}

void bloomail::BaseClient::Smtp::login(const std::string &username,
                                       const std::string &password) {
  if (this->isLoggedIn) {
    logger::error("Already logged in with " + this->rawUsername +
                      ". Attempt to login with " + this->username,
                  "void bloomail::BaseClient::Smtp::login(const std::string "
                  "&username, const std::string &password)");
  }

  this->rawUsername = username;
  this->username = brewtils::base64::encode(username);
  this->password = brewtils::base64::encode(password);
  this->startTls();
  this->authenticate();
  this->isLoggedIn = true;
}

void bloomail::BaseClient::Smtp::send(const std::string &cmd) {
  if (this->debug) {
    logger::debug((this->tlsConnected ? "[TLS] " : "[TCP] ") + cmd);
  }
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
    logger::debug((this->tlsConnected ? "[TLS] " : "[TCP] ") + response);
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
}

void bloomail::BaseClient::Smtp::sendEmail() {
  this->validate();

  this->send("MAIL FROM:<" + this->rawUsername + ">");
  this->recv();

  for (const auto &recipient : this->toRecipients) {
    this->send("RCPT TO:<" + recipient + ">");
    this->recv();
  }
  for (const auto &recipient : this->ccRecipients) {
    this->send("RCPT TO:<" + recipient + ">");
    this->recv();
  }
  for (const auto &recipient : this->bccRecipients) {
    this->send("RCPT TO:<" + recipient + ">");
    this->recv();
  }

  this->send("DATA");
  this->recv();

  std::string header = "Subject: " + this->subject + "\r\n";

  header += "To: ";
  bool first = true;
  for (const auto &r : this->toRecipients) {
    if (!first)
      header += ", ";
    header += "<" + r + ">";
    first = false;
  }
  header += "\r\n";

  if (!this->ccRecipients.empty()) {
    header += "Cc: ";
    first = true;
    for (const auto &r : this->ccRecipients) {
      if (!first)
        header += ", ";
      header += "<" + r + ">";
      first = false;
    }
    header += "\r\n";
  }

  header += "Content-Type: text/plain; charset=UTF-8\r\n";
  header += "\r\n";
  this->send(header + brewtils::string::replace(message, "\n", "\r\n") +
             "\r\n.");
  this->recv();

  return this->clear();
}