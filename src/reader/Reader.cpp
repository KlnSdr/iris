//
// Created by kilian on 29.11.23.
//

#include "Reader.hpp"

void Reader::read() {
    char mainBuffer[config::bufferSize];
    char smolBuffer[config::bufferSize / 2 + 1];
    unsigned int mainBufferOffset = 0;

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(mainBuffer, config::bufferSize, mainBufferOffset);

        // do stuff with buffer
        mainBufferOffset = Package::package(mainBuffer, config::bufferSize - sizeLeft);

        if (mainBufferOffset > 0) {
            std::copy(mainBuffer + (config::bufferSize - sizeLeft - mainBufferOffset),
                      mainBuffer + (config::bufferSize - sizeLeft), smolBuffer);
        }

        std::cout.write(mainBuffer, config::bufferSize - sizeLeft);
    }

}
