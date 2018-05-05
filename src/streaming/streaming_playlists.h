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
  void addPlaylist(std::shared_ptr<RemotePlaylist> playlist);
  std::shared_ptr<std::vector<std::shared_ptr<RemotePlaylist>>> getPlaylists();

 private:
  std::shared_ptr<std::vector<std::shared_ptr<RemotePlaylist>>> playlists;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
