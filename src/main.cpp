#include "io.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    const int bufferSize = 256;
    char buffer[bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, bufferSize);
        std::cout.write(buffer, bufferSize - sizeLeft);
    }
    return 0;
}
