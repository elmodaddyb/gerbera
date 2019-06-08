/*GRB*

Gerbera - https://gerbera.io/

    icon_config.cc - this file is part of Gerbera.

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
/// \file icon_config.cc


#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <zmm/zmm.h>
#include <mxml/mxml.h>

#include "icon_config.h"
#include "gerbera_icon.h"
#include "image_helper.h"
#include "icon_loader.h"


using namespace zmm;
using namespace mxml;

IconConfig::IconConfig() {
  _loadingType = IconLoader::Type::None;
}
IconConfig::IconConfig(const Ref<Element> &config) {
  IconLoader iconLoader;
  _loadingType = iconLoader.identifyLoadingType(config);
  _icons = iconLoader.loadIconList(config);
}

IconLoader::Type IconConfig::type() {
  return _loadingType;
}

std::vector<std::shared_ptr<GerberaIcon>>& IconConfig::icons() {
  return _icons;
}