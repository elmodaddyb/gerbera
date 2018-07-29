#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "configured_playlist.h"


ConfiguredPlaylist::ConfiguredPlaylist(std::string url) : url(std::move(url)) {}

std::string ConfiguredPlaylist::getUrl() {
  return this->url;
}