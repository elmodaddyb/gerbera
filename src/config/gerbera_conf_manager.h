/*GRB*
  Gerbera - https://gerbera.io/

  gerbera_conf_manager.h - this file is part of Gerbera.

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

/// \file gerbera_conf_manager.h

#ifndef GERBERA_ICONFIGMANAGER_H

#define GERBERA_ICONFIGMANAGER_H
#include <streaming/streaming_options.h>


class GerberaConfManager {

public:
    virtual ~GerberaConfManager() = default;
#ifdef ONLINE_SERVICES
    virtual std::shared_ptr<StreamingOptions> getStreamingOptions() = 0;
#endif
};


#endif //GERBERA_ICONFIGMANAGER_H
