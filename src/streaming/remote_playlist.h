#ifndef GERBERA_REMOTE_PLAYLIST_H
#define GERBERA_REMOTE_PLAYLIST_H

#include <string>

class RemotePlaylist {
 public:
  void setUrl(std::string url);
  std::string getUrl();
  void setContent(std::string content);
  std::string getContent();
  std::string readLine();
  std::vector<std::string> getContentVector();

 private:
  std::string url;
  std::string content;
  std::vector<std::string> contentVector;
  unsigned long lineNumber = 0;
};

#endif //GERBERA_REMOTE_PLAYLIST_H
