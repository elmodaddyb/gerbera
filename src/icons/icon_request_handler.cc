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
#include "icon_request_handler.h"

IconRequestHandler::IconRequestHandler() : RequestHandler() {

}

void IconRequestHandler::getInfo(IN const char *filename, OUT UpnpFileInfo *info) {

}


zmm::Ref<IOHandler>
IconRequestHandler::open(IN const char *filename, IN enum UpnpOpenFileMode mode, IN zmm::String range) {
  uint8_t* ptr_image = nullptr;
  size_t size_image = std::size_t{0L};
  zmm::Ref<IOHandler> h(new MemIOHandler(ptr_image, size_image));
  return h;
}