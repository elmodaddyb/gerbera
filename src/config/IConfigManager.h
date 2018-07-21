//
// Created by Eamonn Buss on 7/14/18.
//

#ifndef GERBERA_ICONFIGMANAGER_H

#define GERBERA_ICONFIGMANAGER_H
#include <zmm/zmmf.h>

#include <streaming/streaming_options.h>


class IConfigManager {

public:
    virtual ~IConfigManager() {}
#ifdef ONLINE_SERVICES
    virtual std::shared_ptr<StreamingOptions> getStreamingOptions() = 0;
#endif
};


#endif //GERBERA_ICONFIGMANAGER_H
