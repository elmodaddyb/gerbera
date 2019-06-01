/*GRB*

Gerbera - https://gerbera.io/

    image_helper.h - this file is part of Gerbera.

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
/// \file image_helper.h

#ifndef GERBERA_IMAGE_HELPER_H
#define GERBERA_IMAGE_HELPER_H

#include <string>
#include <Magick++/Blob.h>

struct _imageDetails {
    bool valid;
    size_t width;
    size_t height;
    size_t depth;
};
typedef struct _imageDetails imageDetails;

class ImageHelper {
public:
    ImageHelper() = default;
    ~ImageHelper() = default;
#ifdef HAVE_IMAGEMAGICK
    imageDetails readFromMagick(const std::string &path);
    void convertTo(const std::string &orig, Magick::Blob &dest, const std::string &mimeType);
#endif
#ifdef HAVE_LIBEXIF
    imageDetails readFromExif(const std::string &path);
#endif
#ifdef HAVE_MAGIC
    std::string mimeFromMagic(const std::string &path);
#endif
};


#endif //GERBERA_IMAGE_HELPER_H
