#include <b15f/b15f.h>
#include "listener.hpp"
#include "sender.hpp"
#include <iostream>

const int freq = 1;

void initDataBuffer() {
    std::string value;
    std::getline(std::cin, value);
    Sender::setDataBuffer(value);
}

int main() {
    initDataBuffer();

    B15F& drv = B15F::getInstance();
    drv.setRegister(&DDRA, 0x0F);
    Sender::reset(drv);
    // drv.delay_ms(2000);

    while(1) {
        Reader::read(drv);
        drv.delay_ms(freq);

        // write
        Sender::send(drv);
        
        //read
        Reader::read(drv);
        drv.delay_ms(freq);
    }
}
