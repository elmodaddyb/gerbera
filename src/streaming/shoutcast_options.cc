#include "shoutcast_options.h"

ShoutcastOptions::ShoutcastOptions(std::string baseUrl, std::string devId, bool enabled) {
  this->baseUrl = baseUrl;
  this->devId = devId;
  this->enabled = enabled;
}

ShoutcastOptions::~ShoutcastOptions() {}

std::string ShoutcastOptions::getBaseUrl() {
  return baseUrl;
}

std::string ShoutcastOptions::getDevId() {
  return devId;
}

bool ShoutcastOptions::isEnabled() {
  return enabled;
}



