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
int Reader::ghosting = 0;
int Reader::lastPackageId = 0;

/**
 * @brief Calculates the checksum of the data buffer and prints it.
 *
 * This method calculates the checksum of the data buffer and compares it with the checksum read from the data buffer.
 * If the data buffer is shorter than the checksum size, an info message is logged and the method returns false.
 * The checksum is calculated by summing up the values of the checksum bytes in the data buffer.
 * The checksum bytes are the last bytes in the data buffer, with the number of bytes being determined by the checksum size.
 * The calculated checksum is then compared with the checksum read from the data buffer.
 * If the calculated checksum and the read checksum are equal, a debug message is logged, the data buffer is printed to the standard output, and the method returns true.
 * If the calculated checksum and the read checksum are not equal, a debug message and an info message are logged and the method returns false.
 * Regardless of the result, the data buffer is cleared at the end of the method.
 *
 * @return A bool indicating whether the calculated checksum and the read checksum are equal. Returns true if the checksums are equal, false otherwise.
 */
bool Reader::calculateCheckSumAndPrint() {
    Logger::info("databuffer.size = " + std::to_string(dataBuffer.size()));
    if (dataBuffer.size() < checkSumsize) {
        Logger::error("verworfen: zu kurz");
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
        Logger::error("nicht gleich");
        Logger::info("verworfen, weil crc falsch");
    }

    dataBuffer.clear();
    Logger::debug("======");

    Logger::info("set checkSumIsFOCKINGtheSame to " + std::to_string(isValidPackage));
    Config::checkSumIsFOCKINGtheSame = isValidPackage;

    return true;
}

/**
 * @brief Reads data from a specified channel and processes it.
 *
 * This method reads data from a specified channel and processes it according to the communication protocol.
 * If the isPrimarySend parameter is true, the method checks if the read value is an acknowledgement or resend request.
 * If the read value is an acknowledgement, the everythingIsOkiDoki static member of the Config class is set to true.
 * If the read value is a resend request, the everythingIsOkiDoki static member of the Config class is set to false.
 * The method then sets the direction of the specified channel to output mode.
 * If the isPrimarySend parameter is false, the method processes the read value according to the communication protocol.
 * The method checks for control characters and handles them accordingly.
 * If the read value is a start of packet character, the method sets the beginbool static member to true and the endbool static member to false.
 * If the read value is an end of packet character, the method sets the endbool static member to true and the beginbool static member to false.
 * The method then calculates the checksum of the data buffer and prints it.
 * If the calculated checksum and the read checksum are equal, the checkSumIsFOCKINGtheSame static member of the Config class is set to true.
 * The method then sets the direction of the specified channel to output mode.
 * If the read value is not a control character, the method adds it to the data buffer.
 *
 * @param channel The channel to read from. This should be an int representing the channel.
 * @param isPrimarySend A boolean indicating whether the method should check for an acknowledgement or resend request. If true, the method checks for an acknowledgement or resend request. If false, the method processes the read value according to the communication protocol.
 */
void Reader::read(int channel, bool isPrimarySend) {
    char value = Connector::getInstance().readChannel(channel);

    if (isPrimarySend) {
        if (value == ControlChars::OK || value == ControlChars::RESEND) {
            ghosting = 0;
            if (value == ControlChars::OK) {
                Logger::info("alles oki doki");
            } else {
                Logger::info("resend");
            }
            Logger::info("got val on primary send:" + Helper::charToHex(value));
            Logger::info("set everythingIsOkiDoki to " + std::to_string(value == ControlChars::OK));
            Config::everythingIsOkiDoki = value == ControlChars::OK;

            if (channel == Config::CHANNEL_A) {
                Config::a_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            } else {
                Config::b_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            }
            return;
        } else if (ghosting > 11) {
            ghosting = 0;
            if (channel == Config::CHANNEL_A) {
                Config::a_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            } else {
                Config::b_isWrite = true;
                Helper::setChannel(channel, true, Connector::getInstance().getDrv());
            }
            return;
        }
        ghosting++;

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

        char packageId = dataBuffer.at(0) & 0xFF;
        Logger::debug("packageId: " + std::to_string(packageId));
        dataBuffer.erase(dataBuffer.begin());

        if (packageId < lastPackageId && lastPackageId == 4) {
            lastPackageId = 0;
            Logger::debug("resetting lastPackageId");
        }
        if (packageId <= lastPackageId) {
            Logger::debug("verworfen: gleiche packageId");
            return;
        }

        bool sendResponse = calculateCheckSumAndPrint();

        if (!sendResponse) {
            return;
        }

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = true;
            Helper::setChannel(channel, true, Connector::getInstance().getDrv());
        } else {
            Config::b_isWrite = true;
            Helper::setChannel(channel, true, Connector::getInstance().getDrv());
        }

        Logger::debug("=== end ===");
        return;
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
                    "neuer Char:    " + Helper::charToHex(buffer) + " -> " + static_cast<char>(buffer));
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
