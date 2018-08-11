/*GRB*
  Gerbera - https://gerbera.io/

  downloader.h - this file is part of Gerbera.

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

/// \file downloader.h
#include <string>
#include <memory>
#include <gerbera/url.h>

#ifndef GERBERA_DOWNLOADER_H
#define GERBERA_DOWNLOADER_H

class Downloader {
 public:
  virtual ~Downloader() {};
  virtual std::string download(const std::shared_ptr<gerbera::URL>& url) = 0;
};

#endif //GERBERA_DOWNLOADER_H
