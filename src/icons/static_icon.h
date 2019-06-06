/*GRB*

Gerbera - https://gerbera.io/

    static_icon.h - this file is part of Gerbera.

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
/// \file static_icon.h

#ifndef GERBERA_STATIC_ICON_H
#define GERBERA_STATIC_ICON_H

#include "icon.h"
#include "gerbera_icon.h"
#include "io_handler.h"

class StaticIcon: public Icon {
public:
    explicit StaticIcon(const std::shared_ptr<GerberaIcon> &icon);
    off_t fileLength() override;
    time_t lastModified() override;
    int isDirectory() override;
    int isReadable() override;
    DOMString contentType() override;
    zmm::Ref<IOHandler> handler() override;
private:
    std::shared_ptr<GerberaIcon> icon;
    struct stat statbuf;
    int statReturn;
};


#endif //GERBERA_STATIC_ICON_H
