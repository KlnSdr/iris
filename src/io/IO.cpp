#include "IO.hpp"

bool IO::_hasData = true;

/**
 * @brief Checks if there is data available.
 *
 * @return true if there is data available, false otherwise.
 */
bool IO::hasData() {
    // return !std::cin.eof();
    return _hasData;
}

/**
 * Reads a byte from the input.
 *
 * @return The byte read from the input.
 */
char IO::readByte() {
    char tmp;
    std::cin.get(tmp);
    return tmp;
}

/**
 * Reads data from a buffer.
 *
 * @param buffer The buffer to read data from.
 * @param size The size of the buffer.
 * @return The number of bytes left in the buffer
 */
unsigned int IO::readBuffer(char *buffer, unsigned int size) {
    while (hasData() && size > 0) {
        char val = readByte();
        if (val == '\0') {
            _hasData = false;
            break;
        }
        *buffer = val;
        buffer++;
        size--;
    }

    return size;
}
