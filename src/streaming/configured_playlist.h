#ifndef GERBERA_REMOTE_PLAYLIST_H
#define GERBERA_REMOTE_PLAYLIST_H

#include <string>

class ConfiguredPlaylist {
 public:
  ConfiguredPlaylist() = default;
  explicit ConfiguredPlaylist(std::string url);
  virtual ~ConfiguredPlaylist() = default;
  std::string getUrl();
 private:
  std::string url;
};

#endif //GERBERA_REMOTE_PLAYLIST_H
