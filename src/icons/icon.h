/*GRB*

Gerbera - https://gerbera.io/

    icon.h - this file is part of Gerbera.

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
/// \file icon.h

#ifndef GERBERA_ICON_H
#define GERBERA_ICON_H

#include <sys/stat.h>
#include <ixml.h>
#include <io_handler.h>

class Icon {
public:
    virtual ~Icon() = default;
    virtual off_t fileLength() = 0;
    virtual time_t lastModified() = 0;
    virtual int isDirectory() = 0;
    virtual int isReadable() = 0;
    virtual DOMString contentType() = 0;
    virtual zmm::Ref<IOHandler> handler() = 0;
};

#endif //GERBERA_ICON_H
