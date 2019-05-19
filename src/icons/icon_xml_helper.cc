/*GRB*

Gerbera - https://gerbera.io/

    icon_xml_helper.cc - this file is part of Gerbera.

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
/// \file icon_xml_helper.cc

#include <mxml/mxml.h>
#include "icon_xml_helper.h"
#include "icon_config.h"

using namespace zmm;
using namespace mxml;

zmm::Ref<Element> IconXmlHelper::generateDescList(IconConfig* config) {
  return generateDescList();
}

zmm::Ref<Element> IconXmlHelper::generateDescList() {
  Ref<Element> iconList(new Element(_("iconList")));

  Ref<Element> icon120_png(new Element(_("icon")));
  icon120_png->appendTextChild(_("mimetype"), _(DESC_ICON_PNG_MIMETYPE));
  icon120_png->appendTextChild(_("width"), _("120"));
  icon120_png->appendTextChild(_("height"), _("120"));
  icon120_png->appendTextChild(_("depth"), _("24"));
  icon120_png->appendTextChild(_("url"), _(DESC_ICON120_PNG));
  iconList->appendElementChild(icon120_png);

  Ref<Element> icon120_bmp(new Element(_("icon")));
  icon120_bmp->appendTextChild(_("mimetype"), _(DESC_ICON_BMP_MIMETYPE));
  icon120_bmp->appendTextChild(_("width"), _("120"));
  icon120_bmp->appendTextChild(_("height"), _("120"));
  icon120_bmp->appendTextChild(_("depth"), _("24"));
  icon120_bmp->appendTextChild(_("url"), _(DESC_ICON120_BMP));
  iconList->appendElementChild(icon120_bmp);

  Ref<Element> icon120_jpg(new Element(_("icon")));
  icon120_jpg->appendTextChild(_("mimetype"), _(DESC_ICON_JPG_MIMETYPE));
  icon120_jpg->appendTextChild(_("width"), _("120"));
  icon120_jpg->appendTextChild(_("height"), _("120"));
  icon120_jpg->appendTextChild(_("depth"), _("24"));
  icon120_jpg->appendTextChild(_("url"), _(DESC_ICON120_JPG));
  iconList->appendElementChild(icon120_jpg);

  Ref<Element> icon48_png(new Element(_("icon")));
  icon48_png->appendTextChild(_("mimetype"), _(DESC_ICON_PNG_MIMETYPE));
  icon48_png->appendTextChild(_("width"), _("48"));
  icon48_png->appendTextChild(_("height"), _("48"));
  icon48_png->appendTextChild(_("depth"), _("24"));
  icon48_png->appendTextChild(_("url"), _(DESC_ICON48_PNG));
  iconList->appendElementChild(icon48_png);

  Ref<Element> icon48_bmp(new Element(_("icon")));
  icon48_bmp->appendTextChild(_("mimetype"), _(DESC_ICON_BMP_MIMETYPE));
  icon48_bmp->appendTextChild(_("width"), _("48"));
  icon48_bmp->appendTextChild(_("height"), _("48"));
  icon48_bmp->appendTextChild(_("depth"), _("24"));
  icon48_bmp->appendTextChild(_("url"), _(DESC_ICON48_BMP));
  iconList->appendElementChild(icon48_bmp);

  Ref<Element> icon48_jpg(new Element(_("icon")));
  icon48_jpg->appendTextChild(_("mimetype"), _(DESC_ICON_JPG_MIMETYPE));
  icon48_jpg->appendTextChild(_("width"), _("48"));
  icon48_jpg->appendTextChild(_("height"), _("48"));
  icon48_jpg->appendTextChild(_("depth"), _("24"));
  icon48_jpg->appendTextChild(_("url"), _(DESC_ICON48_JPG));
  iconList->appendElementChild(icon48_jpg);

  Ref<Element> icon32_png(new Element(_("icon")));
  icon32_png->appendTextChild(_("mimetype"), _(DESC_ICON_PNG_MIMETYPE));
  icon32_png->appendTextChild(_("width"), _("32"));
  icon32_png->appendTextChild(_("height"), _("32"));
  icon32_png->appendTextChild(_("depth"), _("8"));
  icon32_png->appendTextChild(_("url"), _(DESC_ICON32_PNG));
  iconList->appendElementChild(icon32_png);

  Ref<Element> icon32_bmp(new Element(_("icon")));
  icon32_bmp->appendTextChild(_("mimetype"), _(DESC_ICON_BMP_MIMETYPE));
  icon32_bmp->appendTextChild(_("width"), _("32"));
  icon32_bmp->appendTextChild(_("height"), _("32"));
  icon32_bmp->appendTextChild(_("depth"), _("8"));
  icon32_bmp->appendTextChild(_("url"), _(DESC_ICON32_BMP));
  iconList->appendElementChild(icon32_bmp);

  Ref<Element> icon32_jpg(new Element(_("icon")));
  icon32_jpg->appendTextChild(_("mimetype"), _(DESC_ICON_JPG_MIMETYPE));
  icon32_jpg->appendTextChild(_("width"), _("32"));
  icon32_jpg->appendTextChild(_("height"), _("32"));
  icon32_jpg->appendTextChild(_("depth"), _("8"));
  icon32_jpg->appendTextChild(_("url"), _(DESC_ICON32_JPG));
  iconList->appendElementChild(icon32_jpg);

  return iconList;
}
