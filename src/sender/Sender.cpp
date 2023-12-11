#include "Sender.hpp"

// char Sender::let[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
std::string Sender::rawData = "";
int Sender::index = 0;
int Sender::checkSumme = 0;
char Sender::lastNibble = ControlChars::PCK_START;
std::vector<char> Sender::data = {};

void Sender::sendNibble(char value, B15F& drv, int channel) {
    // drv.setRegister(&PORTA, (PORTA & 0xF0) | (value & 0x0F));
    drv.setRegister(&PORTA, (value | (value << 4)) & channel);
    // drv.delay_ms(500);
    // std::cout << "+" << std::hex << static_cast<int>(value) << std::endl;
}

void Sender::setDataBuffer(std::string newData){
    rawData = newData;
    preprocess();
}

void Sender::preprocess() {
    for (int i = 0; i < 50; i++) {
       data.push_back(0);
    }
    data.push_back(ControlChars::PCK_START);

    checkSumme = Helper::calcChecksum(rawData);
    std::cout << "Checksumme: "  << checkSumme << std::endl;
    rawData.push_back((char) (checkSumme & 0xFF));

    for (int i = 0; i < rawData.length(); i++) {
        char leftNibble = (char)rawData[i] >> 4;
        char rightNibble = (char)rawData[i] & 0x0F;

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
        std::cout << std::hex << (int) data.at(i) << std::endl;
    }
}

void Sender::reset(B15F& drv, int channel) {
    sendNibble(0, drv, channel);
}

void Sender::send(B15F& drv, int channel, bool isPrimarySend) {
    if (!isPrimarySend) {
        sendNibble(Config::checkSumIsFOCKINGtheSame ? ControlChars::OK : ControlChars::RESEND, drv, channel);

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = false;
            Helper::setChannel(channel, false, drv);
        } else {
            Config::b_isWrite = false;
            Helper::setChannel(channel, false, drv);
        }
        return;
    }

    if (Config::everythingIsOkiDoki) {
        return;
    }

    if (index >= data.size()) {
        sendNibble(0, drv, channel);
        index = 0;

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = false;
            Helper::setChannel(channel, false, drv);
        } else {
            Config::b_isWrite = false;
            Helper::setChannel(channel, false, drv);
        }
    } else {
        sendNibble(data.at(index), drv, channel);
    }

    index++;
}
