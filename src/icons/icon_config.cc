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

#include <memory>
#include <vector>
#include <map>
#include <zmm/zmm.h>
#include <mxml/mxml.h>
#include "icon_config.h"
#include "gerbera_icon.h"


using namespace zmm;
using namespace mxml;
IconConfig::IconConfig() {
  _loadingType = unsupported;
}
IconConfig::IconConfig(Ref<Element> config) {
  std::string loadingType = config->getAttribute("type").c_str();
  if(loadingType == "static-list") {
    _loadingType = static_list;
  } else {
    _loadingType = unsupported;
  }
  _icons = std::make_shared<std::vector<std::shared_ptr<GerberaIcon>>>();
  Ref<Element> icons = config->getChildByName(_("icons"));
  Ref<Element> item;
  std::shared_ptr<GerberaIcon> icon;
  for (int e = 0; e < icons->elementChildCount(); e++) {
    item = icons->getElementChild(e);
    std::string path = item->getText().c_str();
    std::string dimension = item->getAttribute(_("dimension")).c_str();
    std::string depth = item->getAttribute(_("depth")).c_str();
    std::string mimeType = item->getAttribute(_("mime-type")).c_str();
    std::string url = item->getAttribute(_("url")).c_str();
    icon = std::make_shared<GerberaIcon>(path, dimension, depth, mimeType, url);
    _icons->push_back(icon);
  }
}

IconConfig::~IconConfig() {
  _icons = nullptr;
}

icon_loading_type IconConfig::getType() {
  return _loadingType;
}

std::shared_ptr<std::vector<std::shared_ptr<GerberaIcon>>> IconConfig::getIcons() {
  return _icons;
}
