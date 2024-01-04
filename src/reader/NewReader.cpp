//
// Created by kilian on 03.01.24.
//

#include "NewReader.hpp"

char NewReader::compareValue = 0;
bool NewReader::isESC1 = false;
bool NewReader::isESC2 = false;
bool NewReader::isESC3 = false;
bool NewReader::inPackage = false;
char NewReader::buffer = 0;
bool NewReader::isSecondNibble = false;
std::vector<char> NewReader::dataBuffer = {};

void NewReader::read(int channel, bool isPrimarySend) {
    char value = Connector::getInstance().readChannel(channel);
    Logger::info("read: " + Helper::charToHex(value));

    if (isPrimarySend) {
        // todo
        // do some stuff
        return;
    }

    if (compareValue == value) {
        return;
    }

    compareValue = value;

    if (value == ControlChars::ESC3 && !isESC1 && !isESC2) {
        isESC3 = true;
        return;
    }

    if (value == ControlChars::PCK_START && !inPackage) {
        initPackage();
        return;
    }

    if (isESC1) {
        value = (~value) & 0x0F;
    }

    if (value == ControlChars::ESC1 && !isESC2 && !isESC3) {
        isESC1 = true;
        return;
    }

    if (value == ControlChars::ESC2 && !isESC1 && !isESC3) {
        isESC2 = true;
        return;
    }


    if(value == ControlChars::PCK_END && inPackage && !isESC2) {
        inPackage = false;
        processPackage();
        return;
    }

    isESC1 = false;
    isESC2 = false;
    isESC3 = false;

    if (!inPackage) {
        return;
    }

    Logger::info("add to buffer");
    buffer += value;
    buffer &= 0xFF;

    if (isSecondNibble) {
        dataBuffer.push_back(buffer);
        Logger::info("add to dataBuffer: " + Helper::charToHex(buffer));

        buffer = 0;
        isSecondNibble = false;
        return;
    }

    buffer <<= 4;
    buffer &= 0xFF;
    isSecondNibble = true;
}

void NewReader::initPackage() {
    compareValue = ControlChars::PCK_START;
    isESC1 = false;
    isESC2 = false;
    isESC3 = false;
    inPackage = true;
    buffer = 0;
    isSecondNibble = false;
    dataBuffer.clear();
}

void NewReader::processPackage() {
    for (char val: dataBuffer) {
        Logger::info(Helper::charToHex(val) + " -> " + val);
    }
    Logger::info(std::string(dataBuffer.begin(), dataBuffer.end()));
    initPackage();
    inPackage = false;
}