#include <brewtils/env.h>

#include <bloomail/clients/gmail.h>
#include <bloomail/version.h>

int main(int argc, char **argv) {
  brewtils::env::init("../.env");
  std::string emailUsername = brewtils::env::get("EMAIL_USERNAME");
  std::string emailPassword = brewtils::env::get("EMAIL_PASSWORD");

  try {
    bloomail::Client::Gmail gmail = bloomail::Client::Gmail(true);
    gmail.login(emailUsername, emailPassword);

    logger::success("Logged in successfully for " + emailUsername + "!");

    std::string version = "v" + std::to_string(BLOOMAIL_VERSION_MAJOR) + "." +
                          std::to_string(BLOOMAIL_VERSION_MINOR) + "." +
                          std::to_string(BLOOMAIL_VERSION_PATCH);

    gmail.addToRecipient("jadit19@gmail.com");
    //   gmail.addCcRecipient("jadit19@gmail.com");
    //   gmail.addBccRecipient("jadit19@gmail.com");
    gmail.setSubject("[Bloomail] Test message")
        .setMessage("Hi!\n\nThis is a test message with some attachments sent "
                    "using bloomail " +
                    version + "\n\nThanks!")
        .addAttachment("../assets/build.png")
        .addAttachment("../assets/start.png");

    gmail.sendEmail();
    logger::success("Email sent successfully, check inbox!");

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    logger::error("Could not send message");
    return EXIT_FAILURE;
  }
}