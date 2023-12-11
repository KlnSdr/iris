#include "Sender.hpp"

// char Sender::let[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
std::string Sender::rawData = "";
int Sender::index = 0;
int Sender::checkSumme = 0;
char Sender::lastNibble = ControlChars::PCK_START;
std::vector<char> Sender::data = {};

void Sender::setDataBuffer(std::string newData) {
    rawData = newData;
    preprocess();
}

void Sender::preprocess() {
    for (int i = 0; i < 50; i++) {
        data.push_back(0);
    }
    data.push_back(ControlChars::PCK_START);

    checkSumme = Helper::calcChecksum(rawData);
    Logger::debug("Checksumme: " + std::to_string(checkSumme));
    rawData.push_back((char) (checkSumme & 0xFF));

    for (int i = 0; i < rawData.length(); i++) {
        char leftNibble = (char) rawData[i] >> 4;
        char rightNibble = (char) rawData[i] & 0x0F;

        if (leftNibble == ControlChars::ESC1 || leftNibble == ControlChars::PCK_END) {
            data.push_back(ControlChars::ESC2);
        } else if (leftNibble == ControlChars::ESC2 || leftNibble == lastNibble) {
            leftNibble = (~leftNibble) & 0x0F;
            if (leftNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC2);
                leftNibble = (~leftNibble) & 0x0F;
            } else {
                data.push_back(ControlChars::ESC1);
            }
        }
        data.push_back(leftNibble);


        if (rightNibble == ControlChars::ESC1 || rightNibble == ControlChars::PCK_END) {
            data.push_back(ControlChars::ESC2);
        } else if (rightNibble == ControlChars::ESC2 || rightNibble == leftNibble) {
            rightNibble = (~rightNibble) & 0x0F;
            if (rightNibble == ControlChars::ESC1) {
                data.push_back(ControlChars::ESC2);
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
        Logger::debug(Helper::charToHex(data.at(i)));
    }
}

void Sender::reset(int channel) {
    Connector::getInstance().writeChannel(channel, 0);
}

void Sender::send(int channel, bool isPrimarySend) {
    if (!isPrimarySend) {
        Connector::getInstance().writeChannel(channel, Config::checkSumIsFOCKINGtheSame ? ControlChars::OK
                                                                                        : ControlChars::RESEND);

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = false;
            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
        } else {
            Config::b_isWrite = false;
            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
        }
        return;
    }

    if (Config::everythingIsOkiDoki) {
        return;
    }

    if (index >= data.size()) {
        reset(channel);
        index = 0;

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = false;
            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
        } else {
            Config::b_isWrite = false;
            Helper::setChannel(channel, false, Connector::getInstance().getDrv());
        }
    } else {
        Connector::getInstance().writeChannel(channel, data.at(index));
    }

    index++;
}
