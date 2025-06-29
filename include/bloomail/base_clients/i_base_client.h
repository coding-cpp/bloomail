#pragma once

#include <brewtils/os.h>
#include <brewtils/string.h>

namespace bloomail {

namespace BaseClient {

class IBaseClient {
protected:
  static const std::string BOUNDARY;

  bool debug;
  bool isLoggedIn;

  std::string username;
  std::string rawUsername;
  std::string password;
  std::string subject;
  std::string message;

  std::set<std::string> toRecipients;
  std::set<std::string> ccRecipients;
  std::set<std::string> bccRecipients;

  std::vector<std::pair<std::string, std::string>> attachments;

  virtual void send(const std::string &cmd) = 0;
  virtual std::string recv() = 0;
  virtual void authenticate() = 0;
  virtual void quit() = 0;

  void clear();
  void validate();

public:
  IBaseClient(bool debug = false);
  ~IBaseClient();

  virtual void login(const std::string &username,
                     const std::string &password) = 0;
  virtual void sendEmail() = 0;

  IBaseClient &addToRecipient(const std::string &recipient);
  IBaseClient &addCcRecipient(const std::string &recipient);
  IBaseClient &addBccRecipient(const std::string &recipient);

  IBaseClient &setSubject(const std::string &subject);
  IBaseClient &setMessage(const std::string &message);

  IBaseClient &addAttachment(const std::string &pathToFile);
  IBaseClient &addAttachment(const std::string &pathToFile,
                             const std::string &fileName);
};

} // namespace BaseClient

} // namespace bloomail