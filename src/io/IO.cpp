#include "IO.hpp"

/**
 * @brief Checks if there is data available.
 *
 * @return true if there is data available, false otherwise.
 */
bool IO::hasData() {
    return !std::cin.eof();
}

/**
 * Reads a byte from the input.
 *
 * @return The byte read from the input.
 */
char IO::readByte() {
    char tmpBuffer[1];
    std::cin.read(tmpBuffer, 1);

    return tmpBuffer[0];
}

/**
 * Reads data from a buffer.
 *
 * @param buffer The buffer to read data from.
 * @param size The size of the buffer.
 * @return The number of bytes left in the buffer
 */
unsigned int IO::readBuffer(char *buffer, unsigned int size) {
    while (size > 0 && hasData()) {
        *buffer = readByte();
        buffer++;
        size--;
    }

    if (size > 0 && !hasData()) {
        size++;
    }

    return size;
}
