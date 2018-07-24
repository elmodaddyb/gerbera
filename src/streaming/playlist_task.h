//
// Created by Eamonn Buss on 7/14/18.
//

#ifndef GERBERA_PLAYLISTTASK_H
#define GERBERA_PLAYLISTTASK_H


#include <task/task.h>
#include "remote_playlist.h"
#include "streaming_content_service.h"
#include <string>

class PlaylistTask : public Task {
public:
    PlaylistTask(std::string url, StreamingContent* streamingContentService);
    ~PlaylistTask() = default;
    void run() override;
    std::string getUrl();
    void setContent(std::string content);
    std::string getContent();
    std::string readLine();
    std::vector<std::string> getContentVector();
private:
    StreamingContent* streamingContentService;
    std::string url;
    std::string content;
    std::vector<std::string> contentVector;
    unsigned long lineNumber = 0;
};


#endif //GERBERA_PLAYLISTTASK_H
