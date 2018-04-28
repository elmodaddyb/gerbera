#ifndef GERBERA_REMOTE_PLAYLIST_H
#define GERBERA_REMOTE_PLAYLIST_H

#include <string>

class RemotePlaylist {
 public:
  void setUrl(std::string url);
  std::string getUrl();

 private:
  std::string url;
};

#endif //GERBERA_REMOTE_PLAYLIST_H
