#include "Package.hpp"

unsigned int Package::package(char *buffer, unsigned int size) {
    // reserve double the max buffer to store nibbles instead of bytes
    const unsigned int tmpBufferSize = 4;
    char sendBuffer[config::bufferSize * 2];
    char tmpBuffer[tmpBufferSize];
    unsigned int tmpBufferIndex = 0;
    unsigned int sendBufferIndex = 0;
    char lastNibble = 0;

    while (size > 0 && sendBufferIndex < config::bufferSize * 2) {
        char currentChar = *buffer;
        std::cout << "currentChar: " << std::hex << currentChar << std::endl;
        char leftNibble = currentChar >> 4;
        char rightNibble = currentChar & 0xF;

        if (leftNibble == control::esc1 || leftNibble == control::packageEnd || leftNibble == control::transmissionEnd) {
            tmpBuffer[tmpBufferIndex] = control::esc2;
            tmpBufferIndex++;
        } else if (leftNibble == control::esc2 || leftNibble == lastNibble) {
            leftNibble = ~leftNibble & 0xF;
            if (leftNibble == control::esc1) {
                tmpBuffer[tmpBufferIndex] = control::esc2;
                leftNibble = ~leftNibble & 0xF;
            } else {
                tmpBuffer[tmpBufferIndex] = control::esc1;
            }
            tmpBufferIndex++;
        }
        tmpBuffer[tmpBufferIndex] = leftNibble;
        tmpBufferIndex++;

        // todo condense this as its the same as above
        if (rightNibble == control::esc1 || rightNibble == control::packageEnd || rightNibble == control::transmissionEnd) {
            tmpBuffer[tmpBufferIndex] = control::esc2;
            tmpBufferIndex++;
        } else if (rightNibble == control::esc2 || rightNibble == leftNibble) {
            rightNibble = ~rightNibble & 0xF;
            if (rightNibble == control::esc1) {
                tmpBuffer[tmpBufferIndex] = control::esc2;
                rightNibble = ~rightNibble & 0xF;
            } else {
                tmpBuffer[tmpBufferIndex] = control::esc1;
            }
            tmpBufferIndex++;
        }
        tmpBuffer[tmpBufferIndex] = rightNibble;
        tmpBufferIndex++;
        lastNibble = rightNibble;

        if (tmpBufferIndex <= size) {
            std::copy(tmpBuffer, tmpBuffer + tmpBufferIndex, sendBuffer + sendBufferIndex);
            sendBufferIndex += tmpBufferIndex;
            tmpBufferIndex = 0;
        } else {
            return size;
        }

        std::cout << "leftNibble: " << std::hex << leftNibble << "\n";
        std::cout << "rightNibble: " << std::hex << rightNibble << "\n";

        buffer++;
        size--;
    }

    return size;
}
