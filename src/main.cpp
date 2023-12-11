#include "io/io.hpp"
#include "config/config.hpp"
#include "SimulatorConnector/SimulatorConnector.hpp"
#include "connector/Connector.hpp"

int main() {
    config::setup();
    Connector *connector = new SimulatorConnector();
    /*
    char buffer[config::bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, config::bufferSize);
        std::cout.write(buffer, config::bufferSize - sizeLeft);
    }*/
    char message[12] = {'H', 'E', 'L', 'L', 'O', '_', 'W', 'O', 'R', 'L', 'D', '!'};

    for (char value : message) {
        char highNibble = (char) value >> 4;
        connector -> writePins({(highNibble & 0x1) != 0, (highNibble & 0x2) >> 1 != 0, (highNibble & 0x4) >> 2 != 0, (highNibble & 0x8) >> 3 != 0});
        connector -> wait();

        connector -> writePins({false, false, false, false});
        connector -> wait();

        connector -> writePins({(value & 0x1) != 0, (value & 0x2) >> 1 != 0, (value & 0x4) >> 2 != 0, (value & 0x8) >> 3 != 0});
        connector -> wait();

        connector -> writePins({false, false, false, false});
        connector -> wait();
    }

    delete connector;
    return 0;
}
