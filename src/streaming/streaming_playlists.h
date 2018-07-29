#ifndef GERBERA_STREAMING_PLAYLISTS_H
#define GERBERA_STREAMING_PLAYLISTS_H

#include <vector>
#include "configured_playlist.h"
#include <memory>

class StreamingPlaylists {
 public:
  StreamingPlaylists();
  ~StreamingPlaylists();
  unsigned long getSize();
  void addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist);
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> getPlaylists();

 private:
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> playlists;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
