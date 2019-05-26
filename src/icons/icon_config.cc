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
#include "image_helper.h"


using namespace zmm;
using namespace mxml;

IconConfig::IconConfig() {
  _loadingType = unsupported;
}
IconConfig::IconConfig(Ref<Element> config) {
  std::string loadingType = config->getAttribute("type").c_str();
  if (loadingType == "static-list") {
    _loadingType = static_list;
  } else if(loadingType == "dynamic-image") {
    _loadingType = dynamic_image;
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
    std::string url = item->getAttribute(_("url")).c_str();

    imageDetails imgDetails = lookupImage(path, _loadingType);
    std::string resolution = lookupResolution(item, imgDetails);
    std::string depth = lookupDepth(item, imgDetails);
    std::string mimeType = lookupMimeType(item, _loadingType);

    icon = std::make_shared<GerberaIcon>(path, resolution, depth, mimeType, url);
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

std::string IconConfig::getAttribute(const zmm::Ref<Element> &el, const std::string &attr, std::string defaultValue){
  std::string result;
  zmm::String value = el->getAttribute(attr);
  if(value == nullptr) {
    result = std::move(defaultValue);
  } else {
    result = value.c_str();
  }
  return result;
}

std::string IconConfig::lookupMimeType(const zmm::Ref<Element> &icon, icon_loading_type loadingType) {
  std::string mime = getAttribute(icon, "mime-type", "");
  if(loadingType == dynamic_image && mime.empty()) {
#ifdef HAVE_MAGIC
    mime = ImageHelper::mimeFromMagic(icon->getText().c_str());
#endif
  }
  return mime;
}
std::string IconConfig::lookupDepth(const zmm::Ref<Element> &icon, imageDetails details) {
  std::string depth = getAttribute(icon, "depth", "");
  if(depth.empty() && details.valid) {
    std::stringstream ss;
    ss << details.depth;
    depth = ss.str();
  }
  return depth;
}
std::string IconConfig::lookupResolution(const zmm::Ref<Element> &icon, imageDetails details) {
  std::string resolution = getAttribute(icon, "resolution", "");
  if(resolution.empty() && details.valid) {
    std::stringstream ss;
    ss << details.width << "x" << details.height;
    resolution = ss.str();
  }
  return resolution;
}
imageDetails IconConfig::lookupImage(const std::string &path, icon_loading_type loadingType) {
  imageDetails details;
  details.valid = false;
  if (loadingType == dynamic_image) {
#ifdef HAVE_IMAGEMAGICK
    details = ImageHelper::readFromMagick(path);
#endif
#ifdef HAVE_LIBEXIF
    if (!details.valid) {
      details = ImageHelper::readFromExif(path);
    }
#endif
  }
  return details;
}
