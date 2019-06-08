/*GRB*

Gerbera - https://gerbera.io/

    icon_loader.cc - this file is part of Gerbera.

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
/// \file icon_loader.cc

#include <fstream>
#include <vector>
#include "gerbera_icon.h"
#include "icon_loader.h"
#include "image_helper.h"

IconLoader::Type IconLoader::identifyLoadingType(const zmm::Ref<Element> &serverConfig) {
  Type type;
  zmm::Ref<Element> icons = serverConfig->getChildByName(_("icons"));
  zmm::Ref<Element> templateIcon = serverConfig->getChildByName(_("icon"));
#ifdef HAVE_IMAGEMAGICK
  if (templateIcon != nullptr && icons == nullptr) {
    type = Dynamic;
  } else if (icons != nullptr) {
    type = Static;
  } else {
    type = None;
  }
#else
  if (icons != nullptr) {
    type = Type::Static;
  } else {
    type = Type::None;
  }
#endif
  return type;
}

std::string IconLoader::getAttribute(const zmm::Ref<Element> &el, const std::string &attr, std::string defaultValue){
  std::string result;
  zmm::String value = el->getAttribute(attr);
  if(value == nullptr) {
    result = std::move(defaultValue);
  } else {
    result = value.c_str();
  }
  return result;
}
std::string IconLoader::lookupMimeType(const zmm::Ref<Element> &icon) {
  std::string mime = getAttribute(icon, "mime-type", "");
  ImageHelper imageHelper;
  if(mime.empty()) {
#ifdef HAVE_MAGIC
    mime = imageHelper.mimeFromMagic(icon->getText().c_str());
#endif
  }
  return mime;
}
std::string IconLoader::lookupDepth(const zmm::Ref<Element> &icon, ImageDetails details) {
  std::string depth = getAttribute(icon, "depth", "");
  if(depth.empty() && details.valid) {
    std::stringstream ss;
    ss << details.depth;
    depth = ss.str();
  }
  return depth;
}
std::string IconLoader::lookupResolution(const zmm::Ref<Element> &icon, ImageDetails details) {
  std::string resolution = getAttribute(icon, "resolution", "");
  if(resolution.empty() && details.valid) {
    std::stringstream ss;
    ss << details.width << "x" << details.height;
    resolution = ss.str();
  }
  return resolution;
}
ImageDetails IconLoader::lookupImage(const std::string &path) {
  ImageDetails details;
  ImageHelper imageHelper;
  std::ifstream f(path);
  if(f.good()){
#ifdef HAVE_IMAGEMAGICK
    details = imageHelper.readFromMagick(path);
#endif
#ifdef HAVE_LIBEXIF
    if (!details.valid) {
      details = imageHelper.readFromExif(path);
    }
#endif
    f.close();
  } else {
    details.valid = false;
  }
  return details;
}

std::vector<std::shared_ptr<GerberaIcon>> IconLoader::loadIconList(const zmm::Ref<Element> &config) {
  std::vector<std::shared_ptr<GerberaIcon>> icons;
  Type loadingType = identifyLoadingType(config);
  if(loadingType == IconLoader::Type::Dynamic) {
    icons = loadDynamicList(config);
  } else if(loadingType == IconLoader::Type::Static) {
    icons = loadStaticList(config);
  }
  return icons;
}
std::vector<std::shared_ptr<GerberaIcon>> IconLoader::loadDynamicList(const zmm::Ref<Element> &config) {
  auto iconList = std::vector<std::shared_ptr<GerberaIcon>>();
  zmm::Ref<Element> templateIcon = config->getChildByName(_("icon"));
  std::string resolution = DEFAULT_ICON_RESOLUTIONS;
  std::string type = DEFAULT_ICON_TYPES;
  std::string path = templateIcon->getText().c_str();
  std::vector<std::string> resolutions = splitList(resolution);
  std::vector<std::string> types = splitList(type);
  // for each resolution, for each type;
  std::shared_ptr<GerberaIcon> icon;
  for(auto const& r: resolutions) {
    for(auto const& t: types) {
      std::stringstream url;
      url << DEFAULT_ICON_HANDLER << "/grb-icon-" << r << "." << t;

      std::stringstream res;
      res << r << "x" << r;

      std::string depth = "24";

      std::string mimeType;
      if(t == "png") {
        mimeType = DESC_ICON_PNG_MIMETYPE;
      } else if (t == "jpg") {
        mimeType = DESC_ICON_JPG_MIMETYPE;
      } else if (t == "bmp") {
        mimeType = DESC_ICON_BMP_MIMETYPE;
      }
      icon = std::make_shared<GerberaIcon>(path, res.str(), depth, mimeType, url.str());
      iconList.push_back(icon);
    }
  }
  return iconList;
}
std::vector<std::shared_ptr<GerberaIcon>> IconLoader::loadStaticList(const zmm::Ref<Element> &config){
  auto iconList = std::vector<std::shared_ptr<GerberaIcon>>();
  zmm::Ref<Element> icons = config->getChildByName(_("icons"));
  zmm::Ref<Element> item;
  std::shared_ptr<GerberaIcon> icon;
  for (int e = 0; e < icons->elementChildCount(); e++) {
    item = icons->getElementChild(e);
    std::string path = item->getText().c_str();
    std::string url = item->getAttribute(_("url")).c_str();

    ImageDetails imgDetails = lookupImage(path);
    std::string resolution = lookupResolution(item, imgDetails);
    std::string depth = lookupDepth(item, imgDetails);
    std::string mimeType = lookupMimeType(item);

    icon = std::make_shared<GerberaIcon>(path, resolution, depth, mimeType, url);
    iconList.push_back(icon);
  }
  return iconList;
}
std::vector<std::string> IconLoader::splitList(const std::string& list) {
  std::vector<std::string> result;
  std::stringstream ss(list);
  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    result.push_back(substr);
  }
  return result;
}
