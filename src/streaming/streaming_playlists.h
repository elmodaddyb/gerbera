#ifndef GERBERA_STREAMING_PLAYLISTS_H
#define GERBERA_STREAMING_PLAYLISTS_H

#include <vector>
#include "configured_playlist.h"
#include <memory>

class StreamingPlaylists {
 public:
  StreamingPlaylists(std::string rootVirtualPath);
  ~StreamingPlaylists();
  unsigned long getSize();
  void addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist);
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> getPlaylists();
  std::string getRootVirtualPath();

 private:
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> playlists;
  std::string rootVirtualPath;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
