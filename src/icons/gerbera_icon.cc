/*GRB*

Gerbera - https://gerbera.io/

    gerbera_icon.cc - this file is part of Gerbera.

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
/// \file gerbera_icon.cc

#include "gerbera_icon.h"

GerberaIcon::GerberaIcon(std::string path, std::string dimension, std::string depth, std::string mimeType): _path(path),
_dimension(dimension), _depth(depth), _mimeType(mimeType){}

std::string GerberaIcon::path(){
  return _path;
}

std::string GerberaIcon::dimension(){
  return _dimension;
}

std::string GerberaIcon::depth(){
  return _depth;
}

std::string GerberaIcon::mimeType(){
  return _mimeType;
}
