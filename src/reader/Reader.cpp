#include "Reader.hpp"

unsigned int Reader::mainBufferOffset = 0;
char Reader::mainBuffer[config::bufferSize];

bool Reader::read() {
    if (!io::hasData() && Reader::mainBufferOffset == 0) {
        return false;
    }

    unsigned int sizeLeft = io::readBuffer(Reader::mainBuffer, config::bufferSize, Reader::mainBufferOffset);

    // do stuff with buffer
    Reader::mainBufferOffset = Package::package(Reader::mainBuffer, config::bufferSize - sizeLeft);

    if (Reader::mainBufferOffset > 0) {
        std::copy(Reader::mainBuffer + (config::bufferSize - sizeLeft - Reader::mainBufferOffset),
                  Reader::mainBuffer + (config::bufferSize - sizeLeft), Reader::mainBuffer);
    }
    // std::cout.write(mainBuffer, config::bufferSize - sizeLeft);
    return true;
}
