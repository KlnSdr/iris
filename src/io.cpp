#include "io.hpp"

bool io::hasData() {
    return !std::cin.eof();
}

char io::readByte() {
    char tmpBuffer[1];
    std::cin.read(tmpBuffer, 1);

    return tmpBuffer[0];
}

unsigned int io::readBuffer(char *buffer, unsigned int size) {
    while(hasData() && size > 0) {
        *buffer = readByte();
        buffer++;
        size--;
    }

    return size;
}
