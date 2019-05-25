/*GRB*

Gerbera - https://gerbera.io/

    icon_config.h - this file is part of Gerbera.

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
/// \file icon_config.h

#ifndef GERBERA_ICON_CONFIG_H
#define GERBERA_ICON_CONFIG_H
#include <memory>
#include <vector>
#include <map>
#include <icons/gerbera_icon.h>
#include <mxml/mxml.h>

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

using namespace mxml;

enum icon_loading_type {
    static_list,
    dynamic_image,
    unsupported
};

class IconConfig {
private:
    icon_loading_type _loadingType;
    std::shared_ptr<std::vector<std::shared_ptr<GerberaIcon>>> _icons;
public:
    IconConfig();
    explicit IconConfig(zmm::Ref<Element> config);
    ~IconConfig();
    icon_loading_type getType();
    std::shared_ptr<std::vector<std::shared_ptr<GerberaIcon>>> getIcons();
};


#endif //GERBERA_ICON_CONFIG_H
