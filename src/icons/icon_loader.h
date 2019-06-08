/*GRB*

Gerbera - https://gerbera.io/

    icon_loader.h - this file is part of Gerbera.

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
/// \file icon_loader.h

#ifndef GERBERA_ICON_LOADER_H
#define GERBERA_ICON_LOADER_H

#include <memory>
#include <zmm/zmm.h>
#include <mxml/mxml.h>
#include "gerbera_icon.h"
#include "image_helper.h"

#define DESC_ICON_PNG_MIMETYPE "image/png"
#define DESC_ICON_BMP_MIMETYPE "image/x-ms-bmp"
#define DESC_ICON_JPG_MIMETYPE "image/jpeg"
#define DESC_ICON32_PNG "/icons/mt-icon32.png"
#define DESC_ICON32_BMP "/icons/mt-icon32.bmp"
#define DESC_ICON32_JPG "/icons/mt-icon32.jpg"
#define DESC_ICON48_PNG "/icons/mt-icon48.png"
#define DESC_ICON48_BMP "/icons/mt-icon48.bmp"
#define DESC_ICON48_JPG "/icons/mt-icon48.jpg"
#define DESC_ICON120_JPG "/icons/mt-icon120.jpg"
#define DESC_ICON120_PNG "/icons/mt-icon120.png"
#define DESC_ICON120_BMP "/icons/mt-icon120.bmp"
#define DEFAULT_ICON_HANDLER "/content/icons"
#define DEFAULT_ICON_RESOLUTIONS "120,48,32"
#define DEFAULT_ICON_TYPES "jpg,png,bmp"

using namespace mxml;

class IconLoader {
public:
    IconLoader() = default;
    ~IconLoader() = default;
    enum Type {
        Static,
        Dynamic,
        None
    };
    Type identifyLoadingType(const zmm::Ref<Element> &serverConfig);
    std::string getAttribute(const zmm::Ref<Element> &el, const std::string &attr, std::string defaultValue);
    std::string lookupMimeType(const zmm::Ref<Element> &icon);
    std::string lookupResolution(const zmm::Ref<Element> &icon, ImageDetails details);
    std::string lookupDepth(const zmm::Ref<Element> &icon, ImageDetails details);
    ImageDetails lookupImage(const std::string &path);
    std::vector<std::shared_ptr<GerberaIcon>> loadIconList(const zmm::Ref<Element> &config);
    std::vector<std::shared_ptr<GerberaIcon>> loadStaticList(const zmm::Ref<Element> &config);
    std::vector<std::shared_ptr<GerberaIcon>> loadDynamicList(const zmm::Ref<Element> &config);
    std::vector<std::string> splitList(const std::string& list);
};


#endif //GERBERA_ICON_LOADER_H
