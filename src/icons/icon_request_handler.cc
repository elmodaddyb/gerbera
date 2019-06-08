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
#include <memory>
#include <mem_io_handler.h>
#include <file_io_handler.h>
#include <sys/stat.h>
#include "icon_request_handler.h"
#include "icon_config.h"
#include "icon_exception.h"
#include "icon_loader.h"
#include "icon.h"
#include "static_icon.h"
#ifdef HAVE_IMAGEMAGICK
#include "dynamic_icon.h"
#endif

IconRequestHandler::IconRequestHandler(IconConfig *config, std::shared_ptr<ImageHelper> imageHelper) :
        RequestHandler(), config(config), imageHelper(std::move(imageHelper)) {}

void IconRequestHandler::getInfo(IN const char *url, OUT UpnpFileInfo *info) {
  std::shared_ptr<GerberaIcon> grbIcon = findIcon(url);
  if(grbIcon == nullptr) {
    UpnpFileInfo_set_IsReadable(info, 0);
  } else {
    IconLoader::Type type = config->type();
    std::unique_ptr<Icon> icon = generateIcon(grbIcon, type);
    UpnpFileInfo_set_IsReadable(info, icon->isReadable());
    UpnpFileInfo_set_LastModified(info, icon->lastModified());
    UpnpFileInfo_set_IsDirectory(info, icon->isDirectory());
    UpnpFileInfo_set_FileLength(info, icon->fileLength());
    UpnpFileInfo_set_ContentType(info, icon->contentType());
  }
}

zmm::Ref<IOHandler>
IconRequestHandler::open(IN const char *url, IN enum UpnpOpenFileMode mode, IN zmm::String range) {
  zmm::Ref<IOHandler> io_handler;
  std::shared_ptr<GerberaIcon> grbIcon = findIcon(url);
  if(grbIcon == nullptr) {
    throw IconException("Gerbera Icon not found");
  } else {
    IconLoader::Type type = config->type();
    std::unique_ptr<Icon> icon = generateIcon(grbIcon, type);
    io_handler = icon->handler();
    io_handler->open(mode);
  }
  return io_handler;
}

std::shared_ptr<GerberaIcon> IconRequestHandler::findIcon(const std::string &url) {
  std::shared_ptr<GerberaIcon> result;
  auto icons = config->icons();
  for (const auto &icon : icons) {
   if(url == icon->url()) {
     result = icon;
     break;
   }
  }
  return result;
}

std::unique_ptr<Icon> IconRequestHandler::generateIcon(std::shared_ptr<GerberaIcon> grbIcon, IconLoader::Type type) {
  std::unique_ptr<Icon> theIcon;
  switch(type) {
    case IconLoader::Type::Static: {
      theIcon = std::make_unique<StaticIcon>(grbIcon);
      break;
    }
    case IconLoader::Type::None: {
      throw IconException("Dynamic icon loading not supported");
    }
    case IconLoader::Type::Dynamic: {
#ifdef HAVE_IMAGEMAGICK
      theIcon = std::make_unique<DynamicIcon>(grbIcon);
#else
      theIcon = std::make_unique<StaticIcon>(grbIcon);
#endif
      break;
    }
  }
  return theIcon;
}