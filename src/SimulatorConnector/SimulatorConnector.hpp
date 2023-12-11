#ifndef IRIS_SIMULATORCONNECTOR_HPP
#define IRIS_SIMULATORCONNECTOR_HPP


#include "../connector/Connector.hpp"
#include "../config/config.hpp"
#include "../lib/simClient.hpp"
#include "unistd.h"

class SimulatorConnector : public Connector {
public:
    SimulatorConnector();
    void writePins(std::vector<bool> values) override;
    std::vector<bool> readPins() override;
    void wait() override;
    ~SimulatorConnector() override;
};


#endif //IRIS_SIMULATORCONNECTOR_HPP
