#ifndef GERBERA_UPNP_FILE_INFO_FIXTURE_H
#define GERBERA_UPNP_FILE_INFO_FIXTURE_H
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "upnp_file_info_mock.h"

class UpnpFileInfoFixture: public ::testing::Test{
public:
    UpnpFileInfoFixture(){
      _upnpFileInfoMock.reset(new ::testing::NiceMock<UpnpFileInfoMock>());
    }
    ~UpnpFileInfoFixture(){
      _upnpFileInfoMock.reset();
    }
    virtual void SetUp(){}
    virtual void TearDown(){}

    static std::unique_ptr<UpnpFileInfoMock> _upnpFileInfoMock;
};

std::unique_ptr<UpnpFileInfoMock> UpnpFileInfoFixture::_upnpFileInfoMock;

int UpnpFileInfo_set_IsReadable(UpnpFileInfo *info, int isRead){
  return UpnpFileInfoFixture::_upnpFileInfoMock->UpnpFileInfo_set_IsReadable(info, isRead);
}

int UpnpFileInfo_set_ContentType(UpnpFileInfo *info, const DOMString isRead){
  return UpnpFileInfoFixture::_upnpFileInfoMock->UpnpFileInfo_set_ContentType(info, isRead);
}

int UpnpFileInfo_set_IsDirectory(UpnpFileInfo *info, int isDir){
  return UpnpFileInfoFixture::_upnpFileInfoMock->UpnpFileInfo_set_IsDirectory(info, isDir);
}

int UpnpFileInfo_set_FileLength(UpnpFileInfo *info, off_t length){
  return UpnpFileInfoFixture::_upnpFileInfoMock->UpnpFileInfo_set_FileLength(info, length);
}

int UpnpFileInfo_set_LastModified(UpnpFileInfo *info, time_t time){
  return UpnpFileInfoFixture::_upnpFileInfoMock->UpnpFileInfo_set_LastModified(info, time);
}

#endif //GERBERA_UPNP_FILE_INFO_FIXTURE_H
