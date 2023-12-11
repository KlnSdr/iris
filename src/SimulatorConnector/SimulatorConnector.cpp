//
// Created by kilian on 28.11.23.
//

#include "SimulatorConnector.hpp"

void SimulatorConnector::writePins(std::vector<bool> values) {
    if (values.size() != config::Tx.size()) {
        throw std::invalid_argument("values.size() != config::Tx.size()");
    }

    for (unsigned int i = 0; i < values.size(); i++) {
        std::cout << config::Tx[i] << ": " << values[i] << std::endl;
    }

    SimClient::writePins(config::Tx, values);
}

std::vector<bool> SimulatorConnector::readPins() {
    return {};
}

SimulatorConnector::SimulatorConnector() {
    SimClient::connectSim();
    Util::delay_ms(2000);
}

SimulatorConnector::~SimulatorConnector() {
    SimClient::disconnect();
};
