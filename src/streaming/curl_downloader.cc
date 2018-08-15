/*GRB*
  Gerbera - https://gerbera.io/

  curl_downloader_mock.cc - this file is part of Gerbera.

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

/// \file curl_downloader_mock.cc

#ifdef HAVE_CURL

#include <curl/curl.h>
#include <gerbera/gerbera.h>
#include <gerbera/url.h>
#include "curl_downloader.h"
using namespace gerbera;

std::string CurlDownloader::download(const std::shared_ptr<URL>& url) {
  long retcode;
  CURL *curl_handle;
  curl_handle = curl_easy_init();
  std::string result;
  try {
    result = url->download(&retcode, curl_handle, false, false, true);
    log_debug("cURL downloader response return code: %d\n", retcode);
    if (retcode != 200) {
      result = "";
    }
  } catch (gerbera::Exception &ex) {
    log_error("Exception: %s\n", ex.getMessage().c_str());
    result = "";
  }

  if (curl_handle) {
    curl_easy_cleanup(curl_handle);
  }
  return result;
};

#endif //HAVE_CURL
