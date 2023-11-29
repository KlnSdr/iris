//
// Created by kilian on 29.11.23.
//

#include "Reader.hpp"

void Reader::read() {
    char buffer[config::bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, config::bufferSize);
        std::cout.write(buffer, config::bufferSize - sizeLeft);
    }

}
