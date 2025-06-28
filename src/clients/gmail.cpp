#include <bloomail/clients/gmail.h>

bloomail::Client::Gmail::Gmail(bool debug)
    : bloomail::BaseClient::Smtp("smtp.gmail.com", 587, debug) {}

bloomail::Client::Gmail::~Gmail() { return; }