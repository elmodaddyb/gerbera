/*GRB*

Gerbera - https://gerbera.io/

    dynamic_icon.cc - this file is part of Gerbera.

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
/// \file dynamic_icon.cc

#include "dynamic_icon.h"
#include "image_helper.h"

#ifdef HAVE_IMAGEMAGICK

#include <Magick++/Image.h>
#include <mem_io_handler.h>

DynamicIcon::DynamicIcon(const std::shared_ptr<GerberaIcon> &icon) : icon(icon) {
  statReturn = stat(icon->path().c_str(), &statbuf);
}

off_t DynamicIcon::fileLength() {
  return statReturn == 0 ? calculateFileSize(icon) : 0;
}

time_t DynamicIcon::lastModified() {
  return statReturn == 0 ? statbuf.st_mtime : 0;
}

int DynamicIcon::isDirectory() {
  return statReturn == 0 ? S_ISDIR(statbuf.st_mode) : 0;
}

int DynamicIcon::isReadable() {
  return statReturn == 0 && access(icon->path().c_str(), R_OK) == 0;
}

DOMString DynamicIcon::contentType() {
  return ixmlCloneDOMString(icon->mimeType().c_str());
}

zmm::Ref<IOHandler> DynamicIcon::handler() {
  Magick::Blob blob;
  zmm::Ref<IOHandler> io_handler;
  ImageHelper helper;
  helper.convertTo(icon->path(), blob, icon->mimeType());
  std::vector<unsigned long> wh = splitDimension(icon->resolution());
  Magick::Blob resizedBlob;
  helper.resizeTo(blob, resizedBlob, wh.at(0), wh.at(1));
  io_handler = zmm::Ref<IOHandler>(new MemIOHandler((void*)resizedBlob.data(), resizedBlob.length()));
  return io_handler;
}

size_t DynamicIcon::calculateFileSize(const std::shared_ptr<GerberaIcon> &icon) {
  // TODO: Cache image to a file
  ImageHelper helper;
  Magick::Blob blob;
  helper.convertTo(icon->path(), blob, icon->mimeType());
  std::vector<unsigned long> wh = splitDimension(icon->resolution());
  Magick::Blob resizedBlob;
  helper.resizeTo(blob, resizedBlob, wh.at(0), wh.at(1));
  return resizedBlob.length();
}

std::vector<unsigned long> DynamicIcon::splitDimension(std::string dimension) {
  std::string delimiter = "x";
  std::vector<unsigned long> wh;

  size_t pos = 0;
  std::string token;
  while ((pos = dimension.find(delimiter)) != std::string::npos) {
    token = dimension.substr(0, pos);
    wh.push_back(std::stoul(token, nullptr, 0));
    dimension.erase(0, pos + delimiter.length());
  }
  wh.push_back(std::stoul(dimension, nullptr, 0));
  return wh;
}
#endif