/*GRB*

Gerbera - https://gerbera.io/

    image_helper.cc - this file is part of Gerbera.

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
/// \file image_helper.cc

#include "image_helper.h"
#include "icon_config.h"
#include <tools.h>

#ifdef HAVE_IMAGEMAGICK
#include <ImageMagick-7/Magick++.h>
#endif

#ifdef HAVE_LIBEXIF
#include <libexif/exif-data.h>
#include <metadata/libexif_handler.h>
#define BUFLEN 4096
char exif_buffer[BUFLEN];
#define exif_getvalue(arg) exif_entry_get_value(arg, exif_buffer, BUFLEN)
#endif

#ifdef HAVE_IMAGEMAGICK
ImageDetails ImageHelper::readFromMagick(const std::string &path) {
  Magick::Image myIconImage;
  ImageDetails details;
  try {
    myIconImage.read(path);
    size_t width = myIconImage.baseColumns();
    size_t height = myIconImage.baseRows();
    size_t depth = myIconImage.depth();
    details.width = width;
    details.height = height;
    details.depth = depth;
    details.valid = true;
  } catch (std::exception & e) {
    details.valid = false;
  }
  return details;
}

void ImageHelper::convertTo(const std::string &orig, Magick::Blob &dest, const std::string &mimeType) {
  std::string convertToType;
  if(mimeType == DESC_ICON_PNG_MIMETYPE) {
    convertToType = "PNG";
  } else if (mimeType == DESC_ICON_JPG_MIMETYPE) {
    convertToType = "JPEG";
  } else if (mimeType == DESC_ICON_BMP_MIMETYPE) {
    convertToType = "BMP";
  }
  Magick::Image image(orig);
  image.magick(convertToType);
  image.write(&dest);
}

void ImageHelper::resizeTo(const Magick::Blob &fromBlob, Magick::Blob &toBlob, size_t width, size_t height) {
  Magick::Image fromImage(fromBlob);
  fromImage.filterType(Magick::FilterType::LanczosFilter);
  Magick::Geometry newSize(width, height);
  newSize.aspect(true);
  fromImage.resize(newSize);
  fromImage.write(&toBlob);
}
#endif

#ifdef HAVE_LIBEXIF
ImageDetails ImageHelper::readFromExif(const std::string &path) {
  ImageDetails details;
  std::stringstream imageX;
  std::stringstream imageY;
  size_t width;
  size_t height;
  ExifData* ed;
  ed = exif_data_new_from_file(path.c_str());
  if(ed != nullptr) {
    ExifEntry *entry;
    for (int i = 0; i < EXIF_IFD_COUNT; i++) {
      ExifContent *content = ed->ifd[i];
      if (content) {
        entry = exif_content_get_entry(content, EXIF_TAG_PIXEL_X_DIMENSION);
        if (entry != nullptr) {
          imageX << (char *) exif_getvalue(entry);
        }
        entry = exif_content_get_entry(content, EXIF_TAG_PIXEL_Y_DIMENSION);
        if (entry != nullptr) {
          imageY << (char *) exif_getvalue(entry);
        }
      }
    }
    imageX >> width;
    imageY >> height;
    details.width = width;
    details.height = height;
    details.depth = 8; // TODO: from where to get this value?
    details.valid = true;
    exif_data_unref(ed);
  } else {
    details.valid = false;
  }
  return details;
}
#endif

#ifdef HAVE_MAGIC
std::string ImageHelper::mimeFromMagic(const std::string &path) {
  std::string mime;
  zmm::String magicMime = getMIMETypeFromFile(path);
  mime = std::string(magicMime.c_str());
  return mime;
}
#endif