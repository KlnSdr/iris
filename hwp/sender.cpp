#include "sender.hpp"

const char Sender::ESC = 0xA;
const char Sender::BEGIN = 0x5;
const char Sender::END = 0x7;
const char Sender::ESC2 = 0xD;
// char Sender::let[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
std::string Sender::rawData = "";
int Sender::index = 0;
char Sender::lastNibble = BEGIN;
std::vector<char> Sender::data = {};

void Sender::sendNibble(char value, B15F& drv) {
    // drv.setRegister(&PORTA, (PORTA & 0xF0) | (value & 0x0F));
    drv.setRegister(&PORTA, value & 0x0F);
    // drv.delay_ms(500);
    // std::cout << "+" << std::hex << static_cast<int>(value) << std::endl;
}

void Sender::setDataBuffer(std::string newData) {
    rawData = newData;
    preprocess();
}

void Sender::preprocess() {
    for (int i = 0; i < 5; i++) {
        data.push_back(1);
    }

    data.push_back(BEGIN);

    for (int i = 0; i < rawData.length(); i++) {
        int leftNibble = (int)rawData[i] >> 4;
        int rightNibble = (int)rawData[i] & 0x0F;

        if (leftNibble == ESC || leftNibble == END) {
            data.push_back(ESC2);
        } else if (leftNibble == ESC2 || leftNibble == lastNibble) {
            leftNibble = (~leftNibble) & 0xF;
            if (leftNibble == ESC) {
                data.push_back(ESC2);
                leftNibble = (~leftNibble) & 0xF;
            } else {
                data.push_back(ESC);
            }
        }
        data.push_back(leftNibble);


        if (rightNibble == ESC || rightNibble == END) {
            data.push_back(ESC2);
        } else if (rightNibble == ESC2 || rightNibble == leftNibble) {
            rightNibble = (~rightNibble) & 0xF;
            if (rightNibble == ESC) {
                data.push_back(ESC2);
                rightNibble = (~rightNibble) & 0xF;
            } else {
                data.push_back(ESC);
            }
        }
        data.push_back(rightNibble);

        lastNibble = rightNibble;
    }
    data.push_back(END);
}

void Sender::reset(B15F& drv) {
    sendNibble(0, drv);
}

void Sender::send(B15F& drv) {
    if (index >= data.size()) {
        sendNibble(0, drv);
        index = 0;
    } else {
        sendNibble(data.at(index), drv);
    }

    index++;
}
