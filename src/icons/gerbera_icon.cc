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

GerberaIcon::GerberaIcon(std::string path, std::string resolution, std::string depth, std::string mimeType, std::string url):
_path(std::move(path)), _resolution(std::move(resolution)), _depth(std::move(depth)), _mimeType(std::move(mimeType)),
_url(std::move(url)){}

std::string GerberaIcon::path(){
  return _path;
}

std::string GerberaIcon::resolution(){
  return _resolution;
}

std::string GerberaIcon::depth(){
  return _depth;
}

std::string GerberaIcon::mimeType(){
  return _mimeType;
}

std::string GerberaIcon::url() {
  return _url;
}