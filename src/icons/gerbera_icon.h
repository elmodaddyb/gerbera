/*GRB*

Gerbera - https://gerbera.io/

    gerbera_icon.h - this file is part of Gerbera.

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
/// \file gerbera_icon.h

#ifndef GERBERA_GERBERA_ICON_H
#define GERBERA_GERBERA_ICON_H

#include <string>

class GerberaIcon {
public:
    GerberaIcon(std::string path, std::string resolution, std::string depth, std::string mimeType, std::string url);
    std::string path();
    std::string resolution();
    std::string depth();
    std::string mimeType();
    std::string url();
private:
    std::string _path;
    std::string _resolution;
    std::string _depth;
    std::string _mimeType;
    std::string _url;

};


#endif //GERBERA_GERBERA_ICON_H
