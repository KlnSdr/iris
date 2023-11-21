#include "io.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    const int bufferSize = 256;
    char buffer[bufferSize];
//    unsigned int index = 0;

//    while (io::hasData()) {
//        while (index < bufferSize) {
//            char tmp = io::readByte();
//            buffer[index] = tmp;
//            index++;
//
//            if (std::cin.eof()) {
//                break;
//            }
//        }
//
//        if(index > 0) {
//            std::cout.write(buffer, index);
//            index = 0;
//        }
//    }

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, bufferSize);
        std::cout.write(buffer, bufferSize - sizeLeft);
    }
    return 0;
}
