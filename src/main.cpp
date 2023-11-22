#include "io.hpp"
#include "config.hpp"

int main() {
    char buffer[config::bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, config::bufferSize);
        std::cout.write(buffer, config::bufferSize - sizeLeft);
    }
    return 0;
}
