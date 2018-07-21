//
// Created by Eamonn Buss on 7/14/18.
//

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "playlist_task.h"

PlaylistTask::PlaylistTask(std::string url) {
  this->url = url;
}

void PlaylistTask::run() {
  std::this_thread::sleep_for(std::chrono::seconds(3));
  printf("\n%s\n", this->url.c_str());
}

std::string PlaylistTask::getUrl() {
  return this->url;
}

void PlaylistTask::setContent(std::string content) {
  this->content = content;
  std::istringstream f(content.c_str());
  std::string line;
  while (std::getline(f, line)) {
    contentVector.push_back(line);
  }
}

std::string PlaylistTask::getContent() {
  return this->content;
}

std::vector<std::string> PlaylistTask::getContentVector() {
  return this->contentVector;
}

std::string PlaylistTask::readLine() {
  if(contentVector.size() > 0 && contentVector.size() >= lineNumber) {
    return contentVector.at(lineNumber++);
  } else {
    throw std::out_of_range ("End of playlist reached");
  }
}
