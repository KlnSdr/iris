//
// Created by kilian on 03.01.24.
//

#include "NewReader.hpp"

char NewReader::compareValue = 0;
bool NewReader::isESC1 = false;
bool NewReader::isESC2 = false;
bool NewReader::isESC3 = false;
bool NewReader::inPackage = false;
bool NewReader::didValidateMessage = false;
char NewReader::buffer = 0;
bool NewReader::isSecondNibble = false;
std::vector<char> NewReader::dataBuffer = {};

void NewReader::read(int channel) {
    char value = Connector::getInstance().readChannel(channel);
    Logger::info("read: " + Helper::charToHex(value));

//    if (isPrimarySend) {
//        if (!didValidateMessage) {
//            if (value == ControlChars::OK || value == ControlChars::RESEND) {
//                if (value == ControlChars::OK) {
//                    Logger::info("alles oki doki");
//                    Helper::readNextBufferAndPackage();
//                } else {
//                    Logger::info("resend");
//                }
//                Logger::info("got val on primary send:" + Helper::charToHex(value));
//                Logger::info("set everythingIsOkiDoki to " + std::to_string(value == ControlChars::OK));
//                didValidateMessage = true;
//            }
//        } else {
//            if (channel == Config::CHANNEL_A) {
//                Config::a_isWrite = true;
//                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
//            } else {
//                Config::b_isWrite = true;
//                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
//            }
//            didValidateMessage = false;
//        }
//        return;
//    }

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


    if (value == ControlChars::PCK_END && inPackage && !isESC2) {
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
    if (dataBuffer.empty()) {
        Logger::error("dataBuffer is empty");
        return;
    }

    char packageType = dataBuffer.at(0);
    dataBuffer.erase(dataBuffer.begin());

    Logger::error("packageType: " + Helper::charToHex(packageType));

    if (packageType == PackageType::DATA_PKG) {
        processDataPackage();
    } else if (packageType == PackageType::RESPONSE_PKG) {
        Logger::info("process response package");
        processResponsePackage();
    } else {
        Logger::error("packageType is not valid");
    }

    initPackage();
    inPackage = false;
}

void NewReader::processDataPackage() {
    for (char val: dataBuffer) {
        Logger::info(Helper::charToHex(val) + " -> " + val);
    }

    char checkSum = extractChecksum();
    bool checkSumsMatch = Helper::validateMessage(dataBuffer, checkSum);

    Config::checkSumIsFOCKINGtheSame = checkSumsMatch;

    std::vector<char> responsePkg = {};

    if (!checkSumsMatch) {
        Logger::error("Checksumme stimmt nicht überein");
        responsePkg.push_back(ControlChars::RESEND);
    } else {
        std::cout << std::string(dataBuffer.begin(), dataBuffer.end());
        responsePkg.push_back(ControlChars::OK);
    }

    Sender::addToSendQueue(PackageType::RESPONSE_PKG, responsePkg);
}

void NewReader::processResponsePackage() {
    char checkSum = extractChecksum();
    bool checkSumsMatch = Helper::validateMessage(dataBuffer, checkSum);

    char responseCode = dataBuffer.at(0);
    if (responseCode == ControlChars::OK) {
        Logger::info("alles oki doki");
        Helper::readNextBufferAndPackage();
    } else if (responseCode == ControlChars::RESEND) {
        Logger::info("resend");
        Sender::addToSendQueue(PackageType::DATA_PKG, Sender::getLastDataPackagePls());
    } else {
        Logger::error("responseCode is not valid");
    }
}

char NewReader::extractChecksum() {
    if (dataBuffer.empty()) {
        Logger::error("dataBuffer is empty");
        return 0;
    }

    char checksum = dataBuffer.at(dataBuffer.size() - 1);
    dataBuffer.pop_back();
    return checksum;
}
