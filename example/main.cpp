#include <brewtils/env.h>

#include <bloomail/clients/gmail.h>
#include <bloomail/version.h>

int main(int argc, char **argv) {
  brewtils::env::init("../.env");
  std::string emailUsername = brewtils::env::get("EMAIL_USERNAME");
  std::string emailPassword = brewtils::env::get("EMAIL_PASSWORD");

  bloomail::Client::Gmail gmail = bloomail::Client::Gmail(true);
  gmail.login(emailUsername, emailPassword);

  logger::success("Logged in successfully for " + emailUsername + "!");

  std::string version = "v" + std::to_string(BLOOMAIL_VERSION_MAJOR) + "." +
                        std::to_string(BLOOMAIL_VERSION_MINOR) + "." +
                        std::to_string(BLOOMAIL_VERSION_PATCH);

  gmail.addToRecipient("jadit19@gmail.com");
  gmail.setSubject("[Bloomail] Test message");
  gmail.setMessage("Hi!\n\nThis is a test message sent using bloomail " +
                   version + "\n\nThanks!");
  gmail.sendEmail();

  return EXIT_SUCCESS;
}