#include "streaming_playlists.h"


StreamingPlaylists::StreamingPlaylists(std::string rootVirtualPath): rootVirtualPath(std::move(rootVirtualPath)) {
  playlists = std::make_shared<std::vector<std::unique_ptr<ConfiguredPlaylist>>>();
}

StreamingPlaylists::~StreamingPlaylists() {}

unsigned long StreamingPlaylists::getSize() {
  return playlists->size();
}

void StreamingPlaylists::addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist) {
  playlists->push_back(std::move(playlist));
}

std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> StreamingPlaylists::getPlaylists() {
  return playlists;
}

std::string StreamingPlaylists::getRootVirtualPath() {
  return rootVirtualPath;
}
