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
#include <Magick++/Image.h>
#include "icon_request_handler.h"
#include "icon_config.h"
#include "icon_exception.h"

IconRequestHandler::IconRequestHandler(IconConfig *config, std::shared_ptr<ImageHelper> imageHelper) :
        RequestHandler(), config(config), imageHelper(imageHelper) {}

void IconRequestHandler::getInfo(IN const char *url, OUT UpnpFileInfo *info) {
  struct stat statbuf;
  std::shared_ptr<GerberaIcon> icon = findIcon(url);
  if(icon == nullptr) {
    UpnpFileInfo_set_IsReadable(info, 0);
  } else {
    std::string iconPath = icon->path();
    int ret = stat(iconPath.c_str(), &statbuf);
    if (ret == 0) {
      if (access(iconPath.c_str(), R_OK) == 0) {
        icon_loading_type type = config->getType();
        switch(type) {
          case static_list: {
            UpnpFileInfo_set_IsReadable(info, 1);
            UpnpFileInfo_set_LastModified(info, statbuf.st_mtime);
            UpnpFileInfo_set_IsDirectory(info, S_ISDIR(statbuf.st_mode));
            UpnpFileInfo_set_FileLength(info, statbuf.st_size);
            UpnpFileInfo_set_ContentType(info, ixmlCloneDOMString(icon->mimeType().c_str()));
            break;
          }
          case dynamic_image: {
            // TODO: Cache image to a file
            Magick::Blob blob;
            imageHelper->convertTo(iconPath, blob, icon->mimeType());
            UpnpFileInfo_set_IsReadable(info, 1);
            UpnpFileInfo_set_LastModified(info, statbuf.st_mtime);
            UpnpFileInfo_set_IsDirectory(info, S_ISDIR(statbuf.st_mode));
            UpnpFileInfo_set_FileLength(info, blob.length());
            UpnpFileInfo_set_ContentType(info, ixmlCloneDOMString(icon->mimeType().c_str()));
            break;
          }
          case unsupported: {
            UpnpFileInfo_set_IsReadable(info, 0);
          }
        }
      } else {
        UpnpFileInfo_set_IsReadable(info, 0);
      }
    } else {
      UpnpFileInfo_set_IsReadable(info, 0);
    }
  }
}


zmm::Ref<IOHandler>
IconRequestHandler::open(IN const char *url, IN enum UpnpOpenFileMode mode, IN zmm::String range) {
  struct stat statbuf;
  zmm::Ref<IOHandler> io_handler;
  std::shared_ptr<GerberaIcon> icon = findIcon(url);
  icon_loading_type type = config->getType();
  if(icon == nullptr) {
    throw IconException("Could not find icon by url: " + std::string(url));
  } else {
    switch(type) {
      case static_list: {
        int ret = stat(icon->path().c_str(), &statbuf);
        if (ret == 0) {
          if (access(icon->path().c_str(), R_OK) == 0) {
            io_handler = zmm::Ref<IOHandler>(new FileIOHandler(icon->path()));
            io_handler->open(mode);
          } else {
            throw IconException("Could not access icon by path: " + std::string(icon->path()));
          }
        } else {
          throw IconException("Could not access icon by path: " + std::string(icon->path()));
        }
        break;
      }
      case dynamic_image: {
        Magick::Blob blob;
        imageHelper->convertTo(icon->path(), blob, icon->mimeType());
        io_handler = zmm::Ref<IOHandler>(new MemIOHandler((void*)blob.data(), blob.length()));
        io_handler->open(mode);
        break;
      }
      case unsupported: {
        throw IconException("Dynamic icon loading not supported");
      }
    }
  }
  return io_handler;
}

std::shared_ptr<GerberaIcon> IconRequestHandler::findIcon(const std::string &url) {
  std::shared_ptr<GerberaIcon> result;
  auto icons = config->getIcons();
  for (const auto &icon : icons) {
   if(icon->url() == url) {
     result = icon;
     break;
   }
  }
  return result;
}