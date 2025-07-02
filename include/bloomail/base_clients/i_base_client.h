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

  void addToRecipient(const std::string &recipient);
  void addCcRecipient(const std::string &recipient);
  void addBccRecipient(const std::string &recipient);

  void setSubject(const std::string &subject);
  void setMessage(const std::string &message);

  void addAttachment(const std::string &pathToFile);
  void addAttachment(const std::string &pathToFile,
                     const std::string &fileName);
};

} // namespace BaseClient

} // namespace bloomail