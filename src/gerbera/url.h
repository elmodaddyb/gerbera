/*GRB*
  Gerbera - https://gerbera.io/

  url.h - this file is part of Gerbera.

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

/// \file url.h

#ifdef HAVE_CURL

#include <curl/curl.h>
#include <memory>
#include <string>

#ifndef GERBERA_URL_H
#define GERBERA_URL_H
namespace gerbera {

class URL {
 public:
  URL(std::string url);
  ~URL() = default;

  /// \brief This is a simplified version of the File_Info class as used
  /// in libupnp.
  class Stat {
   public:
    /// \brief Initalizes the class with given values, the values
    /// can not be changed afterwards.
    ///
    /// \param size size of the media in bytes
    /// \param mimetype mime type of the media
    Stat(std::string url, off_t size, std::string mimetype) {
      this->url = url;
      this->size = size;
      this->mimetype = mimetype;
    }

    std::string getURL() { return url; }
    off_t getSize() { return size; }
    std::string getMimeType() { return mimetype; }

   protected:
    std::string url;
    off_t size;
    std::string mimetype;
  };

  /// \brief downloads either the content or the headers to the buffer.
  ///
  /// This function uses an already initialized curl_handle, the reason
  /// is, that curl might keep the connection open if we do subsequent
  /// requests to the same server.
  ///
  /// \param curl_handle an initialized and ready to use curl handle
  /// \param URL
  /// \param only_header set true if you only want the header and not the
  /// body
  /// \param vebose enable curl verbose option
  std::string download(long *HTTP_retcode,
                       CURL *curl_handle = NULL,
                       bool only_header = false,
                       bool verbose = false,
                       bool redirect = false);

  std::shared_ptr<Stat> getInfo(CURL *curl_handle = NULL);

  /// \brief This function is installed as a callback for libcurl, when
  /// we download data from a remote site.
  static size_t dl(void *buf, size_t size, size_t nmemb, void *data);
 private:
  std::string url;
};

}
#endif //GERBERA_URL_H
#endif //HAVE_CURL
