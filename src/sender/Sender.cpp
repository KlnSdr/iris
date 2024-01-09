#include "Sender.hpp"

// char Sender::let[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
std::vector<char> Sender::rawData = {};
int Sender::index = 0;
char Sender::checkSumme = 0;
char Sender::lastNibble = ControlChars::PCK_START;
std::vector<char> Sender::data = {};
bool Sender::didSendOkResend = false;
std::queue<std::tuple<PackageType, std::vector<char>>> Sender::sendQueue = {};
std::vector<char> Sender::lastDataPackage = {};

/**
 * @brief Preprocesses the raw data for transmission.
 *
 * This method prepares the rawData for transmission according to the communication protocol.
 * It first fills the data vector with 50 zeros and then adds the start of packet character.
 * It then calculates the checksum of the rawData and adds it to the end of the rawData.
 * The method then iterates over each character in the rawData. For each character, it separates the left and right nibbles.
 * If the left nibble is a control character or the same as the last nibble, it is escaped according to the communication protocol and added to the data vector.
 * The same process is then repeated for the right nibble.
 * After all characters in the rawData have been processed, the end of packet character is added to the data vector.
 * If the last nibble is the end of packet character, it is escaped according to the communication protocol.
 * Finally, the method logs the hexadecimal representation of each byte in the data vector.
 */
void Sender::preprocess(PackageType type) {
    bool isDataPackage = type == PackageType::DATA_PKG;

    if (isDataPackage) {
        lastDataPackage = rawData;
    }

    data.clear();
    for (int i = 0; i < 10; i++) {
        data.push_back(0);
    }
    data.push_back(ControlChars::PCK_START);

    checkSumme = Helper::calcChecksum(rawData);
    Logger::info("Checksumme: " + Helper::charToHex(checkSumme));
    rawData.push_back((char) (checkSumme & 0xFF));

    rawData.insert(rawData.begin(), type);

    for (int i = 0; i < rawData.size(); i++) {
        Logger::info(Helper::charToHex(rawData.at(i)));
    }
    Logger::info("////////////////////////////////");

    for (int i = 0; i < rawData.size(); i++) {
        char leftNibble = ((char) rawData[i] >> 4) & 0x0F;
        char rightNibble = (char) rawData[i] & 0x0F;

        if (leftNibble == ControlChars::ESC1 || leftNibble == ControlChars::PCK_END || leftNibble == ControlChars::ESC3) {
            data.push_back(ControlChars::ESC2);
        } else if (leftNibble == ControlChars::ESC2 || leftNibble == lastNibble) {
            leftNibble = (~leftNibble) & 0x0F;
            if (leftNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC2);
                leftNibble = (~leftNibble) & 0x0F;
            } else if (lastNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC3);
                leftNibble = (~leftNibble) & 0x0F;
            } else {
                data.push_back(ControlChars::ESC1);
            }
        }
        data.push_back(leftNibble);


        if (rightNibble == ControlChars::ESC1 || rightNibble == ControlChars::PCK_END || rightNibble == ControlChars::ESC3) {
            data.push_back(ControlChars::ESC2);
        } else if (rightNibble == ControlChars::ESC2 || rightNibble == leftNibble) {
            rightNibble = (~rightNibble) & 0x0F;
            if (rightNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC2);
                rightNibble = (~rightNibble) & 0x0F;
            } else if (leftNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC3);
                rightNibble = (~rightNibble) & 0x0F;
            } else {
                data.push_back(ControlChars::ESC1);
            }
        }
        data.push_back(rightNibble);

        lastNibble = rightNibble;
    }
    if (lastNibble == ControlChars::PCK_END) {
        data.push_back(ControlChars::ESC1);
        data.push_back((~ControlChars::PCK_END) & 0x0F);
    } else {
        data.push_back(ControlChars::PCK_END);
    }

    for (int i = 0; i < data.size(); i++) {
        Logger::info(Helper::charToHex(data.at(i)));
    }
    Logger::info("###########################");
}

/**
 * @brief Resets the specified channel.
 *
 * This method resets the specified channel by writing a zero to it using the Connector class's writeChannel method.
 * The Connector class's getInstance method is used to get a reference to the Connector instance.
 *
 * @param channel The channel to reset. This should be an int representing the channel.
 */
void Sender::reset(int channel) {
    Connector::getInstance().writeChannel(channel, 0);
}

/**
 * @brief Sends data over a specified channel.
 *
 * This method sends data over a specified channel according to the communication protocol.
 * If the isPrimarySend parameter is false, the method writes an acknowledgement or resend request to the channel depending on the checkSumIsFOCKINGtheSame static member of the Config class.
 * The method then sets the direction of the specified channel to input mode.
 * If the isPrimarySend parameter is true, the method checks if everythingIsOkiDoki static member of the Config class is true. If it is, the method returns.
 * If the index is greater than or equal to the size of the data vector, the method resets the specified channel and sets the index to 0.
 * The method then sets the direction of the specified channel to input mode.
 * If the index is less than the size of the data vector, the method writes the data at the current index to the channel.
 * The index is then incremented.
 *
 * @param channel The channel to send data over. This should be an int representing the channel.
 * @param isPrimarySend A boolean indicating whether the method should write an acknowledgement or resend request to the channel. If true, the method sends data over the channel according to the communication protocol. If false, the method writes an acknowledgement or resend request to the channel.
 */
void Sender::send(int channel) {
//    if (!isPrimarySend) {
//        if (!didSendOkResend) {
//            Logger::error("sending ack: checkTheSame: " + std::to_string(Config::checkSumIsFOCKINGtheSame));
//            Connector::getInstance().writeChannel(channel, Config::checkSumIsFOCKINGtheSame ? ControlChars::OK
//                                                                                            : ControlChars::RESEND);
//            didSendOkResend = true;
//        } else {
//            if (channel == Config::CHANNEL_A) {
//                Config::a_isWrite = false;
//            } else {
//                Config::b_isWrite = false;
//            }
//            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
//            didSendOkResend = false;
//            reset(channel);
//        }
//        return;
//    }

    if (sendQueue.empty() && data.empty()) {
        Logger::info("sendQueue is empty");
        return;
    } else if (!sendQueue.empty() && data.empty()) {
        std::tuple<PackageType, std::vector<char>> packageDef = sendQueue.front();
        rawData = std::get<1>(packageDef);
        preprocess(std::get<0>(packageDef));
        sendQueue.pop();
    }

    if (index < data.size()) {
        Logger::info("send: " + Helper::charToHex(data.at(index)));
        Connector::getInstance().writeChannel(channel, data.at(index));
    }

    index++;

    if (index == data.size()) {
        // reset(channel);
        index = 0;
        data = {};

//        if (channel == Config::CHANNEL_A) {
//            Config::a_isWrite = false;
//            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
//        } else {
//            Config::b_isWrite = false;
//            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
//        }
    }
}

void Sender::addToSendQueue(PackageType type, const std::vector<char>& newData) {
    sendQueue.emplace(type, newData);
}

std::vector<char> Sender::getLastDataPackagePls() {
    return lastDataPackage;
}
