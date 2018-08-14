#ifndef GERBERA_CONFIG_MGR_MOCK_H
#define GERBERA_CONFIG_MGR_MOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <config/gerbera_conf_manager.h>

using namespace ::testing;

class ConfigManagerMock : public GerberaConfManager {
public:
    MOCK_METHOD0(getStreamingOptions, std::shared_ptr<StreamingOptions>());
};

#endif //GERBERA_CONFIG_MGR_MOCK_H
