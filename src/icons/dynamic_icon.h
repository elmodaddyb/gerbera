/*GRB*

Gerbera - https://gerbera.io/

    dynamic_icon.h - this file is part of Gerbera.

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
/// \file dynamic_icon.h

#ifndef GERBERA_DYNAMIC_ICON_H
#define GERBERA_DYNAMIC_ICON_H

#include <sys/stat.h>
#include "icon.h"
#include "gerbera_icon.h"

#ifdef HAVE_IMAGEMAGICK
class DynamicIcon: public Icon {
public:
    DynamicIcon(const std::shared_ptr<GerberaIcon> &icon);
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
    size_t calculateFileSize(const std::shared_ptr<GerberaIcon> &icon);
    std::vector<unsigned long> splitDimension(std::string dimension);
};
#endif


#endif //GERBERA_DYNAMIC_ICON_H
