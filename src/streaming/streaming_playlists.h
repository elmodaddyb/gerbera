#ifndef GERBERA_STREAMING_PLAYLISTS_H
#define GERBERA_STREAMING_PLAYLISTS_H

#include <vector>
#include "remote_playlist.h"
#include <memory>

using namespace std;

class StreamingPlaylists {
 public:
  StreamingPlaylists();
  ~StreamingPlaylists();
  unsigned long getSize();
  void addPlaylist(shared_ptr<RemotePlaylist> &playlist);
  shared_ptr<vector<shared_ptr<RemotePlaylist>>> getPlaylists();

 private:
  shared_ptr<vector<shared_ptr<RemotePlaylist>>> playlists;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
