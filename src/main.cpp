#include "io/io.hpp"
#include "config/config.hpp"

int main() {
    config::setup();
    char buffer[config::bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, config::bufferSize);
        std::cout.write(buffer, config::bufferSize - sizeLeft);
    }
    return 0;
}
