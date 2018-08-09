#ifndef GERBERA_REMOTE_PLAYLIST_H
#define GERBERA_REMOTE_PLAYLIST_H

#include <string>

class ConfiguredPlaylist {
 public:
  ConfiguredPlaylist() = default;
  explicit ConfiguredPlaylist(std::string url, std::string name);
  virtual ~ConfiguredPlaylist() = default;
  std::string getUrl();
  std::string getName();
 private:
  std::string url;
  std::string name;
};

#endif //GERBERA_REMOTE_PLAYLIST_H
