#ifndef GERBERA_UPNP_FILE_INFO_MOCK_H
#define GERBERA_UPNP_FILE_INFO_MOCK_H
#include "gmock/gmock.h"
#include "FileInfo.h"
#include "ixml.h"

class UpnpFileInfoMock{
public:
    virtual ~UpnpFileInfoMock() = default;

    MOCK_METHOD2(UpnpFileInfo_set_ContentType, int(UpnpFileInfo *info, const DOMString domStr));
    MOCK_METHOD2(UpnpFileInfo_set_LastModified, int(UpnpFileInfo *info, time_t time));
    MOCK_METHOD2(UpnpFileInfo_set_IsReadable, int(UpnpFileInfo *info, int isRead));
    MOCK_METHOD2(UpnpFileInfo_set_IsDirectory, int(UpnpFileInfo *info, int isDir));
    MOCK_METHOD2(UpnpFileInfo_set_FileLength, int(UpnpFileInfo *info, off_t length));
};

#endif //GERBERA_UPNP_FILE_INFO_MOCK_H
