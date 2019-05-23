/*GRB*

Gerbera - https://gerbera.io/

    icon_request_handler.cc - this file is part of Gerbera.

    Copyright (C) 2016-2019 Gerbera Contributors

    Gerbera is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    Gerbera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.

    $Id$
*/
/// \file icon_request_handler.cc
#include <mem_io_handler.h>
#include <file_io_handler.h>
#include <sys/stat.h>
#include "icon_request_handler.h"
#include "icon_config.h"

IconRequestHandler::IconRequestHandler(IconConfig* config) : RequestHandler(), config(config) {}

void IconRequestHandler::getInfo(IN const char *filename, OUT UpnpFileInfo *info) {
  struct stat statbuf;
  std::string path = "";
  int ret = stat(path.c_str(), &statbuf);

  if (access(path.c_str(), R_OK) == 0) {
    UpnpFileInfo_set_IsReadable(info, 1);
  } else {
    UpnpFileInfo_set_IsReadable(info, 0);
  }
  UpnpFileInfo_set_LastModified(info, statbuf.st_mtime);
  UpnpFileInfo_set_IsDirectory(info, S_ISDIR(statbuf.st_mode));
  UpnpFileInfo_set_FileLength(info, statbuf.st_size);
  UpnpFileInfo_set_ContentType(info, ixmlCloneDOMString("image/png"));
}


zmm::Ref<IOHandler>
IconRequestHandler::open(IN const char *filename, IN enum UpnpOpenFileMode mode, IN zmm::String range) {
//  uint8_t* ptr_image = nullptr;
//  size_t size_image = std::size_t{0L};
//  zmm::Ref<IOHandler> h(new MemIOHandler(ptr_image, size_image));
  std::string path = "";
  zmm::Ref<IOHandler> io_handler(new FileIOHandler(path));
  io_handler->open(mode);
  return io_handler;
}