#include <bloomail/base_clients/i_base_client.h>

bloomail::BaseClient::IBaseClient::IBaseClient(bool debug) : debug(debug) {
  return;
}

bloomail::BaseClient::IBaseClient::~IBaseClient() { return; }

void bloomail::BaseClient::IBaseClient::addToRecipient(
    const std::string &recipient) {
  this->toRecipients.insert(recipient);
}

void bloomail::BaseClient::IBaseClient::addCcRecipient(
    const std::string &recipient) {
  this->ccRecipients.insert(recipient);
}

void bloomail::BaseClient::IBaseClient::addBccRecipient(
    const std::string &recipient) {
  this->bccRecipients.insert(recipient);
}

void bloomail::BaseClient::IBaseClient::setSubject(const std::string &subject) {
  this->subject = subject;
}

void bloomail::BaseClient::IBaseClient::setMessage(const std::string &message) {
  this->message = message;
}

void bloomail::BaseClient::IBaseClient::clear() {
  this->subject.clear();
  this->message.clear();
  this->toRecipients.clear();
  this->ccRecipients.clear();
  this->bccRecipients.clear();
}

void bloomail::BaseClient::IBaseClient::validate() {
  if (!this->isLoggedIn) {
    logger::error("You need to be authenticated before sending an email",
                  "void bloomail::BaseClient::IBaseClient::validate()");
  }

  if (this->toRecipients.empty()) {
    logger::error("Empty TO recipients",
                  "void bloomail::BaseClient::IBaseClient::validate()");
  }

  if (this->subject.empty()) {
    logger::error("Empty subject line",
                  "void bloomail::BaseClient::IBaseClient::validate()");
  }

  if (this->message.empty()) {
    logger::error("Empty message",
                  "void bloomail::BaseClient::IBaseClient::validate()");
  }
}