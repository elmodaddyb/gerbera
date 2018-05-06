#ifndef GERBERA_STREAMING_PLAYLISTS_H
#define GERBERA_STREAMING_PLAYLISTS_H

#include <vector>
#include "remote_playlist.h"
#include <memory>

class StreamingPlaylists {
 public:
  StreamingPlaylists();
  ~StreamingPlaylists();
  unsigned long getSize();
  void addPlaylist(std::unique_ptr<RemotePlaylist> playlist);
  std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>> getPlaylists();

 private:
  std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>> playlists;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
