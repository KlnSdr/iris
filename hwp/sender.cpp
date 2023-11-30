#include <b15f/b15f.h>
#include <iostream>
#include <string> 
#include <array>

void sendNibble(char value, B15F& drv) {
    drv.setRegister(&PORTA, value);
    drv.delay_ms(500);
    std::cout << "+" << std::hex << static_cast<int>(value) << std::endl;
}

int main(){
    const char ESC = 0xA;
    const char BEGIN = 0x5;
    const char END = 0x7;
    const char ESC2 = 0xD;

    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0f);

    drv.setRegister(&PORTA, 0x0);
    drv.delay_ms(5000);

    char let[11] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};


    int i = 0;
    char lastNibble = BEGIN;

    sendNibble(BEGIN, drv);

    while( i <= 10 ){
        int leftNibble = (int)let[i] >> 4;
        int rightNibble = (int)let[i] & 0x0F;
        i++;

        if (leftNibble == ESC || leftNibble == END) {
            sendNibble(ESC2, drv);
        } else if (leftNibble == ESC2 || leftNibble == lastNibble) {
            leftNibble = (~leftNibble) & 0xF;
            if (leftNibble == ESC) {
                sendNibble(ESC2, drv);
                leftNibble = (~leftNibble) & 0xF;
            } else {
                sendNibble(ESC, drv);
            }
        }
        sendNibble(leftNibble, drv);


        if (rightNibble == ESC || rightNibble == END) {
            sendNibble(ESC2, drv);
        } else if (rightNibble == ESC2 || rightNibble == leftNibble) {
            rightNibble = (~rightNibble) & 0xF;
            if (rightNibble == ESC) {
                sendNibble(ESC2, drv);
                rightNibble = (~rightNibble) & 0xF;
            } else {
                sendNibble(ESC, drv);
            }
        }
        sendNibble(rightNibble, drv);

        lastNibble = rightNibble;
    }
    sendNibble(END, drv);
    sendNibble(0, drv);
}
