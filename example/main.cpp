#include <brewtils/env.h>

#include <bloomail/clients/gmail.h>

int main(int argc, char **argv) {
  brewtils::env::init("../.env");
  std::string emailUsername = brewtils::env::get("EMAIL_USERNAME");
  std::string emailPassword = brewtils::env::get("EMAIL_PASSWORD");

  bloomail::Client::Gmail gmail = bloomail::Client::Gmail(true);
  gmail.login(emailUsername, emailPassword);

  return EXIT_SUCCESS;
}