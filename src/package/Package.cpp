//
// Created by kilian on 29.11.23.
//

#include "Package.hpp"

unsigned int Package::package(char *buffer, unsigned int size) {
    // reserve double the max buffer to save nibbles instead of bytes
    char sendBuffer[config::bufferSize * 2];
    unsigned int sendBufferIndex = 0;
    char lastNibble = 0;

    while (size > 0 && sendBufferIndex < config::bufferSize * 2) {
        char currentChar = *buffer;
        std::cout << "currentChar: " << std::hex << currentChar << std::endl;
        char leftNibble = currentChar >> 4;
        char rightNibble = currentChar & 0xF;

        if (leftNibble == control::esc1 || leftNibble == control::packageEnd || leftNibble == control::transmissionEnd) {
            sendBuffer[sendBufferIndex] = control::esc2;
            sendBufferIndex++;
        } else if (leftNibble == control::esc2 || leftNibble == lastNibble) {
            leftNibble = ~leftNibble & 0xF;
            if (leftNibble == control::esc1) {
                sendBuffer[sendBufferIndex] = control::esc2;
                leftNibble = ~leftNibble & 0xF;
            } else {
                sendBuffer[sendBufferIndex] = control::esc1;
            }
            sendBufferIndex++;
        }
        sendBuffer[sendBufferIndex] = leftNibble;
        sendBufferIndex++;

        // todo condense this as its the same as above
        if (rightNibble == control::esc1 || rightNibble == control::packageEnd || rightNibble == control::transmissionEnd) {
            sendBuffer[sendBufferIndex] = control::esc2;
            sendBufferIndex++;
        } else if (rightNibble == control::esc2 || rightNibble == leftNibble) {
            rightNibble = ~rightNibble & 0xF;
            if (rightNibble == control::esc1) {
                sendBuffer[sendBufferIndex] = control::esc2;
                rightNibble = ~rightNibble & 0xF;
            } else {
                sendBuffer[sendBufferIndex] = control::esc1;
            }
            sendBufferIndex++;
        }
        sendBuffer[sendBufferIndex] = rightNibble;
        sendBufferIndex++;
        lastNibble = rightNibble;


        std::cout << "leftNibble: " << std::hex << leftNibble << "\n";
        std::cout << "rightNibble: " << std::hex << rightNibble << "\n";

        buffer++;
        size--;
    }

    return size;
}
