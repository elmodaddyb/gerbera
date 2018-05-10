#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "remote_playlist.h"


void RemotePlaylist::setUrl(std::string url) {
  this->url = std::move(url);
}

std::string RemotePlaylist::getUrl() {
  return url;
}

void RemotePlaylist::setContent(std::string content) {
  this->content = content;
  std::istringstream f(content.c_str());
  std::string line;
  while (std::getline(f, line)) {
    contentVector.push_back(line);
  }
}

std::string RemotePlaylist::getContent() {
  return this->content;
}

std::vector<std::string> RemotePlaylist::getContentVector() {
  return this->contentVector;
}

std::string RemotePlaylist::readLine() {
  if(contentVector.size() > 0 && contentVector.size() >= lineNumber) {
    return contentVector.at(lineNumber++);
  } else {
    throw std::out_of_range ("End of playlist reached");
  }
}