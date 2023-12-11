#include "Reader.hpp"

bool Reader::escbool = false;
bool Reader::esc2bool = false;
bool Reader::beginbool = false;
bool Reader::endbool = false;
int Reader::buffer = 0;
int Reader::offset = 0;
int Reader::pause = 0;
int Reader::compareWert = 0;
int Reader::checkSumsize = 1;
std::vector<char> Reader::dataBuffer = {};

bool Reader::calculateCheckSumAndPrint() {
    if (dataBuffer.size() < checkSumsize) {
        Logger::info("verworfen: zu kurz");
        return false;
    }
    Logger::debug("=======");
    for (int i = 0; i < dataBuffer.size(); i++) {
        Logger::debug(Helper::charToHex(dataBuffer.at(i)));
    }
    Logger::debug("---");
    int checkSum = 0;

    for (int i = 0; i < checkSumsize; i++) {
        int checksumByte = dataBuffer.at(dataBuffer.size() - i - 1) & 0xFF;
        checkSum += checksumByte << i;
        Logger::debug(Helper::charToHex(checksumByte));
    }
    dataBuffer.resize(dataBuffer.size() - checkSumsize);

    std::string dataBufferString(dataBuffer.begin(), dataBuffer.end());
    int calcedCheckSum = Helper::calcChecksum(dataBufferString);

    Logger::debug("calced " + std::to_string(calcedCheckSum) + " == read " + std::to_string(checkSum) + "?");

    bool isValidPackage = calcedCheckSum == checkSum;

    if (isValidPackage) {
        Logger::debug("gleich");
        std::cout << dataBufferString << std::endl;
    } else {
        Logger::debug("nicht gleich");
        Logger::info("verworfen, weil crc falsch");
    }

    dataBuffer.clear();
    Logger::debug("======");
    return isValidPackage;
}

void Reader::read(int channel, bool isPrimarySend) {
    char value = Connector::getInstance().readChannel(channel);

    if (isPrimarySend) {
        if (value == ControlChars::OK || value == ControlChars::RESEND) {
            Config::everythingIsOkiDoki = value == ControlChars::OK;

            if (channel == Config::CHANNEL_A) {
                Config::a_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            } else {
                Config::b_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            }
        }
        // todo timer falls lange kein richtiges zeichen anliegt

        return;
    }

    if (compareWert == value) {
        pause++;
        return;
    } else if (pause > 20) {
        buffer = 0;
        endbool = true;
        beginbool = false;
        std::cout << std::endl;
        pause = 0;
        return;
    }
    pause = 0;
    compareWert = value;

    Logger::debug("gelesener Hex: " + Helper::charToHex(value));

    if (escbool == true) {
        value = (~value) & 0xF;
        Logger::debug("inv " + Helper::charToHex(value));
        escbool = false;
    }

    if (value == ControlChars::ESC2 && compareWert == value) {
        esc2bool = true;
    }

    if (value == ControlChars::ESC1 && esc2bool == false) {
        escbool = true;
    }

    if (value == ControlChars::PCK_START && beginbool == false) {
        Logger::debug("=== begin ===");
        beginbool = true;
        endbool = false;
        escbool = false;
        esc2bool = false;
        return;
    }


    if (value == ControlChars::PCK_END && esc2bool == true) {
        endbool = false;
        esc2bool = false;

    } else if (value == ControlChars::PCK_END && esc2bool == false) {
        endbool = true;
        beginbool = false;
        buffer = 0;
        offset = 0;

        bool isValidPackage = calculateCheckSumAndPrint();
        Config::checkSumIsFOCKINGtheSame = isValidPackage;

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = true;
            Helper::setChannel(channel, true, Connector::getInstance().getDrv());
        } else {
            Config::b_isWrite = true;
            Helper::setChannel(channel, true, Connector::getInstance().getDrv());
        }

        Logger::debug("=== end ===");
    }

    if (esc2bool == true && value != ControlChars::ESC2) {
        esc2bool = false;
    }

    Logger::debug("begin: " + std::to_string(beginbool) + "  End: " + std::to_string(endbool) + "  esc2:  " +
                  std::to_string(esc2bool) + "  esc:   " + std::to_string(escbool));
    if (beginbool == true && endbool == false && esc2bool == false && escbool == false) {

        buffer <<= offset;
        Logger::debug("add to buffer: " + Helper::charToHex(value));
        buffer += value;
        offset += 4;

        if (offset > 4) {
            Logger::debug("add to data buffer: " + Helper::charToHex(buffer));
            dataBuffer.push_back(buffer);
            Logger::debug(
                    "neuer Char:    " + Helper::charToHex(buffer) + " -> " + std::to_string(static_cast<char>(buffer)));
            offset = 0;
            buffer = 0;
        }

        if (pause >= 6) {
            buffer = 0;
            offset = 0;
            pause = 0;
        }

    }

}


