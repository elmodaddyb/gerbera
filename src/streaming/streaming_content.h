//
// Created by Eamonn Buss on 7/21/18.
//

#ifndef GERBERA_STREAMING_CONTENT_H
#define GERBERA_STREAMING_CONTENT_H

class StreamingContent {
public:
    virtual ~StreamingContent() = default;
    virtual void processConfiguredPlaylists() = 0;
    virtual void printUrl(std::string url) = 0;
};

#endif //GERBERA_STREAMING_CONTENT_H
