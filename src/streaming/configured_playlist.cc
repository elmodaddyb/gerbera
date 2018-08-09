#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "configured_playlist.h"


ConfiguredPlaylist::ConfiguredPlaylist(std::string url, std::string name) : url(std::move(url)), name(std::move(name)) {}

std::string ConfiguredPlaylist::getUrl() {
  return this->url;
}

std::string ConfiguredPlaylist::getName() {
  return this->name;
}