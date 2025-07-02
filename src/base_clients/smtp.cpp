#include <bloomail/base_clients/smtp.h>

const int bloomail::BaseClient::Smtp::CHUNK_SIZE = 57;

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

void bloomail::BaseClient::Smtp::sendEmail() {
  this->validate();
  this->sendHeaders();
  this->sendAttachments();
  this->send("--" + bloomail::BaseClient::IBaseClient::BOUNDARY + "--\r\n.");
  this->recv();
  this->clear();
  return;
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

void bloomail::BaseClient::Smtp::sendHeaders() {
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

  header += "MIME-Version: 1.0\r\n";
  header += "Content-Type: multipart/mixed; boundary=" +
            bloomail::BaseClient::IBaseClient::BOUNDARY + "\r\n";
  header += "\r\n";
  header += "--" + bloomail::BaseClient::IBaseClient::BOUNDARY + "\r\n";
  header += "Content-Type: text/plain; charset=UTF-8\r\n";
  header += "Content-Transfer-Encoding: 7bit\r\n\r\n";
  header += brewtils::string::replace(this->message, "\n", "\r\n");
  this->send(header);
}

void bloomail::BaseClient::Smtp::sendAttachments() {
  for (const auto &[filePath, fileName] : this->attachments) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      logger::error("Could not open file " + filePath);
      continue;
    }

    std::string attachmentHeader;
    attachmentHeader +=
        "--" + bloomail::BaseClient::IBaseClient::BOUNDARY + "\r\n";
    attachmentHeader +=
        "Content-Type: " + brewtils::os::file::getMimeType(filePath) + "\r\n";
    attachmentHeader +=
        "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n";
    attachmentHeader += "Content-Transfer-Encoding: base64\r\n\r\n";
    this->send(attachmentHeader);

    char buffer[bloomail::BaseClient::Smtp::CHUNK_SIZE];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
      std::string chunk(buffer, file.gcount());
      std::string encoded = brewtils::base64::encode(chunk);
      this->send(encoded);
    }
    this->send("\r\n");
  }
  return;
}