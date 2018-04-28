#ifndef GERBERA_SHOUTCAST_OPTIONS_H
#define GERBERA_SHOUTCAST_OPTIONS_H

#include <string>
class ShoutcastOptions {
 public:
  ShoutcastOptions(std::string baseUrl, std::string devId, bool enabled);
  ~ShoutcastOptions();
  std::string getBaseUrl();
  std::string getDevId();
  bool isEnabled();
 private:
  std::string baseUrl;
  std::string devId;
  bool enabled = false;
};

#endif //GERBERA_SHOUTCAST_OPTIONS_H
