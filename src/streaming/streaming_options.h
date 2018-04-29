#ifndef GERBERA_STREAMING_OPTIONS_H
#define GERBERA_STREAMING_OPTIONS_H

#include <mxml/mxml.h>
#include "streaming_playlists.h"
#include "shoutcast_options.h"
using namespace mxml;

class StreamingOptions {
 public:
  StreamingOptions(zmm::Ref<Element> streamingElement);
  ~StreamingOptions();
  std::shared_ptr<StreamingPlaylists> getPlaylists();
  std::shared_ptr<ShoutcastOptions> getShoutcastOptions();

 private:
  std::shared_ptr<StreamingPlaylists> playlists;
  std::shared_ptr<ShoutcastOptions> shoutcast;
};

#endif //GERBERA_STREAMING_OPTIONS_H
