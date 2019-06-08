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
#include <icons/gerbera_icon.h>
#include <mxml/mxml.h>
#include "image_helper.h"
#include "icon_loader.h"

using namespace mxml;

class IconConfig {
private:
    std::vector<std::shared_ptr<GerberaIcon>> _icons;
    IconLoader::Type _loadingType;
public:
    IconConfig();
    explicit IconConfig(const zmm::Ref<Element> &serverConfig);
    ~IconConfig() = default;
    IconLoader::Type type();
    std::vector<std::shared_ptr<GerberaIcon>>& icons();
};


#endif //GERBERA_ICON_CONFIG_H
