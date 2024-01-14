#include "Helper.hpp"

/**
 * @brief Calculates the checksum of a given string.
 *
 * This method calculates the checksum of a given string by summing up the ASCII values of each character in the string.
 * The sum is then modulo divided by 0xFF to ensure the checksum fits into a single byte.
 * The resulting checksum is returned.
 *
 * @param data The string for which to calculate the checksum. This should be a std::string containing the data.
 * @return The calculated checksum. This is an int representing the checksum of the given string.
 */
char Helper::calcChecksum(std::vector<char> data) {
    unsigned char checkSumme = 0;
    Logger::debug("(((((((((((((((((((((((((((((((");
    for (int i = 0; i < data.size(); i++) {
        Logger::debug(Helper::charToHex(data.at(i)));
        checkSumme += (char) (data.at(i) & 0xFF);
        checkSumme &= 0xFF;
        Logger::debug(" -> " + Helper::charToHex(checkSumme));
    }
    Logger::debug("(((((((((((((((((((((((((((((((");
    return (checkSumme % 0xFF) & 0xFF;
}

/**
 * @brief Sets the direction of a specified channel.
 *
 * This method sets the direction of a specified channel using the driver's getRegister and setRegister methods.
 * The current value of the DDRA register is first retrieved. If the isWrite parameter is true, the channel is set to output mode
 * by bitwise OR'ing the current value of the DDRA register with the channel. If the isWrite parameter is false, the channel is set to input mode
 * by bitwise AND'ing the current value of the DDRA register with the bitwise NOT of the channel.
 *
 * @param channel The channel for which to set the direction. This should be an int representing the channel.
 * @param isWrite A boolean indicating whether the channel should be set to output mode (true) or input mode (false).
 * @param drv A reference to the B15F driver object used to interact with the hardware.
 */
void Helper::setChannel(int channel, bool isWrite, B15F &drv) {
    uint8_t value = drv.getRegister(&DDRA);
    Logger::error("aktuell ddra: " + Helper::charToHex(value));

    if (isWrite) {
        Logger::error("setze " + std::to_string(channel) + " auf output");
    } else {
        Logger::error("setze " + std::to_string(channel) + " auf input");
    }

    if (isWrite) {
        drv.setRegister(&DDRA, value | channel);
        Logger::error("neu ddra: " + Helper::charToHex(value | channel));
    } else {
        drv.setRegister(&DDRA, value & ((~channel) & 0xFF));
        Logger::error("neu ddra: " + Helper::charToHex(value & ((~channel) & 0xFF)));
    }
}

/**
 * @brief Converts a char to a hexadecimal string.
 *
 * This method converts a char to a hexadecimal string using a std::stringstream.
 * The stringstream is first set to output hexadecimal numbers. The width of the output is then set to 2 and the fill character is set to '0'.
 * This ensures that the output will always be 2 characters long, even if the hexadecimal representation of the char is only 1 character long.
 * The char is then cast to an int and inserted into the stringstream.
 * The stringstream is then converted to a std::string and returned.
 *
 * @param chr The char to convert to a hexadecimal string. This should be a char representing the value.
 * @return The hexadecimal representation of the char as a std::string.
 */
std::string Helper::charToHex(char chr) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << std::hex << ((int) chr & 0xFF);
    std::string hexString = ss.str();
    return hexString;
}

/**
 * @brief Initializes the data buffer for transmission.
 *
 * This method reads data from the IO buffer into a local buffer of size Config::bufferSize.
 * The number of bytes read is stored in the variable 'written'.
 * A string 'value' is then created from the local buffer, starting from the beginning of the buffer and ending at the position determined by subtracting 'written' from Config::bufferSize.
 * This string 'value' is then set as the data buffer in the Sender class using the setDataBuffer method.
 */
void Helper::readNextBufferAndPackage() {
    char buffer[Config::bufferSize];
    unsigned int bytesLeft = IO::readBuffer(buffer, Config::bufferSize);
    if (bytesLeft == Config::bufferSize) {
        Logger::info("disable sender");
        return;
    }

    std::vector<char> value(buffer, buffer + (Config::bufferSize - bytesLeft));
    Sender::addToSendQueue(PackageType::DATA_PKG, value);
}

bool Helper::validateMessage(std::vector<char> data, char sendCheckSum) {
    char checkSum = Helper::calcChecksum(
            std::move(data)); // thx CLion for the move suggestion and copilot for this comment
    return checkSum == sendCheckSum;
}
