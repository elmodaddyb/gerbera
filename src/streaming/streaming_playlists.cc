#include "streaming_playlists.h"


StreamingPlaylists::StreamingPlaylists() {
  playlists = std::make_shared<std::vector<std::unique_ptr<RemotePlaylist>>>();
}

StreamingPlaylists::~StreamingPlaylists() {}

unsigned long StreamingPlaylists::getSize() {
  return playlists->size();
}

void StreamingPlaylists::addPlaylist(std::unique_ptr<RemotePlaylist> playlist) {
  playlists->push_back(std::move(playlist));
}

std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>> StreamingPlaylists::getPlaylists() {
  return playlists;
}
