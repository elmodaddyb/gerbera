#include <string>
#include "remote_playlist.h"


void RemotePlaylist::setUrl(std::string url) {
  this->url = std::move(url);
}

std::string RemotePlaylist::getUrl() {
  return url;
}
