/*GRB*
  Gerbera - https://gerbera.io/

  streaming_options.h - this file is part of Gerbera.

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

/// \file streaming_options.h

#ifndef GERBERA_STREAMING_OPTIONS_H
#define GERBERA_STREAMING_OPTIONS_H

#include <memory>
#include <vector>
#include <mxml/mxml.h>

using namespace mxml;

class StreamingOptions {
public:
    class ConfiguredPlaylist {
    public:
        ConfiguredPlaylist() = default;
        explicit ConfiguredPlaylist(std::string url, std::string name, int purgeAfter);
        virtual ~ConfiguredPlaylist() = default;
        std::string getUrl();
        std::string getName();
        int purgeAfter();
    private:
        std::string _url;
        std::string _name;
        int _purgeAfter;
    };

    class Playlists {
    public:
        explicit Playlists(std::string rootVirtualPath, bool updateAtStart);
        ~Playlists() = default;
        unsigned long getSize();
        void addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist);
        std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> getPlaylists();
        std::string getRootVirtualPath();
        bool isUpdateAtStart();

    private:
        std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> confPlaylists;
        std::string _rootVirtualPath;
        bool _updateAtStart;
    };

    class Shoutcast {
    public:
        Shoutcast(std::string baseUrl, std::string devId, bool enabled);
        ~Shoutcast() = default;
        std::string getBaseUrl();
        std::string getDevId();
        bool isEnabled();

    private:
        std::string _baseUrl;
        std::string _devId;
        bool _enabled = false;
    };

    explicit StreamingOptions(zmm::Ref<Element> streamingElement);
    ~StreamingOptions();
    std::shared_ptr<Playlists> playlists();
    std::shared_ptr<Shoutcast> shoutcastOptions();

private:
    std::shared_ptr<Playlists> _playlists;
    std::shared_ptr<Shoutcast> _shoutcast;
};

#endif //GERBERA_STREAMING_OPTIONS_H
