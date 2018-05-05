#include "streaming_playlists.h"


StreamingPlaylists::StreamingPlaylists() {
  playlists = std::make_shared<std::vector<std::shared_ptr<RemotePlaylist>>>();
}

StreamingPlaylists::~StreamingPlaylists() {}

unsigned long StreamingPlaylists::getSize() {
  return playlists->size();
}

void StreamingPlaylists::addPlaylist(std::shared_ptr<RemotePlaylist> playlist) {
  playlists->push_back(playlist);
}

std::shared_ptr<std::vector<std::shared_ptr<RemotePlaylist>>> StreamingPlaylists::getPlaylists() {
  return playlists;
}
