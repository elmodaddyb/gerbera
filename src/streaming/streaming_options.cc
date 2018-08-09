#include "streaming_options.h"

using namespace zmm;

StreamingOptions::StreamingOptions(Ref<Element> streamingConfig) {


  Ref<Element> cfgPlaylists = streamingConfig->getChildByName(_("playlists"));
  std::string rootVirtualPath = cfgPlaylists->getAttribute("root-virtual-path").c_str();
  playlists = std::make_shared<StreamingPlaylists>(rootVirtualPath);
  std::unique_ptr<ConfiguredPlaylist> playlist;
  Ref<Element> item;
  for (int e = 0; e < cfgPlaylists->elementChildCount(); e++) {
    item = cfgPlaylists->getElementChild(e);
    playlist = std::make_unique<ConfiguredPlaylist>(item->getAttribute(_("url")).c_str(), item->getAttribute(_("name")).c_str());
    playlists->addPlaylist(std::move(playlist));
  }

  Ref<Element> cfgShoutcast = streamingConfig->getChildByName(_("shoutcast"));
  std::string baseUrl = cfgShoutcast->getAttribute(_("base-url")).c_str();
  std::string devId = cfgShoutcast->getAttribute(_("dev-id")).c_str();
  std::string enabled = cfgShoutcast->getAttribute(_("enabled")).c_str();
  shoutcast = std::make_shared<ShoutcastOptions>(baseUrl, devId, enabled.compare("yes") == 0);
}

StreamingOptions::~StreamingOptions() {
  playlists = nullptr;
  shoutcast = nullptr;
}


std::shared_ptr<StreamingPlaylists> StreamingOptions::getPlaylists() {
  return playlists;
}

std::shared_ptr<ShoutcastOptions> StreamingOptions::getShoutcastOptions() {
  return shoutcast;
}