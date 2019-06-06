/*GRB*

Gerbera - https://gerbera.io/

    static_icon.cc - this file is part of Gerbera.

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
/// \file static_icon.cc

#include <file_io_handler.h>
#include "gerbera_icon.h"
#include "static_icon.h"

StaticIcon::StaticIcon(const std::shared_ptr<GerberaIcon> &icon) : icon(icon) {
  statReturn = stat(icon->path().c_str(), &statbuf);
}

off_t StaticIcon::fileLength() {
  return statReturn == 0 ? statbuf.st_size : 0;
}

time_t StaticIcon::lastModified() {
  return statReturn == 0 ? statbuf.st_mtime : 0;
}

int StaticIcon::isDirectory() {
  return statReturn == 0 ? S_ISDIR(statbuf.st_mode) : 0;
}

int StaticIcon::isReadable() {
  return statReturn == 0 && access(icon->path().c_str(), R_OK) == 0;
}

DOMString StaticIcon::contentType() {
  return ixmlCloneDOMString(icon->mimeType().c_str());
}

zmm::Ref<IOHandler> StaticIcon::handler() {
  zmm::Ref<IOHandler> io_handler;
  io_handler = zmm::Ref<IOHandler>(new FileIOHandler(icon->path()));
  return io_handler;
}
