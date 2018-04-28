#include "streaming_options.h"

using namespace zmm;

StreamingOptions::StreamingOptions(Ref<Element> streamingConfig) {
  playlists = make_shared<StreamingPlaylists>();

  Ref<Element> cfgPlaylists = streamingConfig->getChildByName(_("playlists"));
  shared_ptr<RemotePlaylist> playlist;
  Ref<Element> item;
  for (int e = 0; e < cfgPlaylists->elementChildCount(); e++) {
    item = cfgPlaylists->getElementChild(e);
    playlist = make_shared<RemotePlaylist>();
    playlist->setUrl(item->getAttribute(_("url")).c_str());
    playlists->addPlaylist(playlist);
  }

  Ref<Element> cfgShoutcast = streamingConfig->getChildByName(_("shoutcast"));
  std::string baseUrl = cfgShoutcast->getAttribute(_("base-url")).c_str();
  std::string devId = cfgShoutcast->getAttribute(_("dev-id")).c_str();
  std::string enabled = cfgShoutcast->getAttribute(_("enabled")).c_str();
  shoutcast = make_shared<ShoutcastOptions>(baseUrl, devId, enabled == "yes");
}


shared_ptr<StreamingPlaylists> StreamingOptions::getPlaylists() {
  return playlists;
}

shared_ptr<ShoutcastOptions> StreamingOptions::getShoutcastOptions() {
  return shoutcast;
}