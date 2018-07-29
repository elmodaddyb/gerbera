

#ifndef GERBERA_PLAYLISTTASK_H
#define GERBERA_PLAYLISTTASK_H


#include <task/task.h>
#include "configured_playlist.h"
#include "streaming_content_service.h"
#include <string>

class PlaylistTask : public Task {
public:
    PlaylistTask(std::string url, StreamingContent* streamingContentService);
    ~PlaylistTask() = default;
    void run() override;
    std::string getUrl();

private:
    StreamingContent* streamingContentService;
    std::string url;
};


#endif //GERBERA_PLAYLISTTASK_H
