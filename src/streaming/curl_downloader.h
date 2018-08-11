/*GRB*
  Gerbera - https://gerbera.io/

  curl_downloader.h - this file is part of Gerbera.

  Copyright (C) 2018 Gerbera Contributors

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

/// \file curl_downloader.h

#ifdef HAVE_CURL

#ifndef GERBERA_CURLDOWNLOADER_H
#define GERBERA_CURLDOWNLOADER_H

#include <string>
#include <gerbera/url.h>
#include "downloader.h"

class CurlDownloader : public Downloader {
 public:
  CurlDownloader() = default;
  ~CurlDownloader() = default;
  std::string download(const std::shared_ptr<gerbera::URL>& url) override;
};

#endif //GERBERA_CURLDOWNLOADER_H
#endif //HAVE_CURL