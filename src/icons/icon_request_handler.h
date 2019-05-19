/*GRB*

Gerbera - https://gerbera.io/

    icon_request_handler.h - this file is part of Gerbera.

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

/// \file icon_request_handler.h
#ifndef GERBERA_ICONREQUESTHANDLER_H
#define GERBERA_ICONREQUESTHANDLER_H


#include <request_handler.h>
#include "icon_config.h"

class IconRequestHandler: public RequestHandler {
protected:
    IconConfig* config;
public:
    explicit IconRequestHandler(IconConfig* config);
    virtual void getInfo(IN const char *filename, OUT UpnpFileInfo *info);
    virtual zmm::Ref<IOHandler> open(IN const char* filename,
                                     IN enum UpnpOpenFileMode mode,
                                     IN zmm::String range);
};


#endif //GERBERA_ICONREQUESTHANDLER_H
