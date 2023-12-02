#include "Package.hpp"

unsigned int Package::packageBufferIndex = 0;
unsigned int Package::packageBufferSize = 0;
char Package::packageBuffer[config::bufferSize * 2];
bool Package::didSendStart = false;
bool Package::didSendEnd = false;

unsigned int Package::package(char *buffer, unsigned int size) {
    Package::packageBufferIndex = 0;
    Package::didSendStart = false;
    Package::didSendEnd = false;

    // reserve double the max buffer to store nibbles instead of bytes
    const unsigned int tmpBufferSize = 4;
    char tmpBuffer[tmpBufferSize];
    unsigned int tmpBufferIndex = 0;
    unsigned int sendBufferIndex = 0;
    char lastNibble = control::packageStart;

    while (size > 0 && sendBufferIndex < config::bufferSize * 2) {
        char currentChar = *buffer;
        char leftNibble = currentChar >> 4;
        char rightNibble = currentChar & 0xF;

        if (leftNibble == control::esc1 || leftNibble == control::packageEnd ||
            leftNibble == control::transmissionEnd) {
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
        if (rightNibble == control::esc1 || rightNibble == control::packageEnd ||
            rightNibble == control::transmissionEnd) {
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

        if (tmpBufferIndex + 1 + sendBufferIndex < config::bufferSize * 2) {
            std::copy(tmpBuffer, tmpBuffer + tmpBufferIndex, Package::packageBuffer + sendBufferIndex);
            sendBufferIndex += tmpBufferIndex;
            tmpBufferIndex = 0;
        } else {
            Package::packageBufferSize = sendBufferIndex;
            return size;
        }

        buffer++;
        size--;
    }

    Package::packageBufferSize = sendBufferIndex;
    return size;
}

bool Package::hasNext() {
    return (Package::packageBufferIndex < Package::packageBufferSize || !Package::didSendEnd) && Package::packageBufferSize > 0;
}

char Package::next() {
    if (!didSendStart) {
        didSendStart = true;
        return control::packageStart;
    } else if (Package::packageBufferIndex >= Package::packageBufferSize && !didSendEnd) {
        didSendEnd = true;
        return control::packageEnd;
    }
    return Package::packageBuffer[Package::packageBufferIndex++];
}
