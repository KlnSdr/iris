#include <b15f/b15f.h>
#include "listener.hpp"
#include "sender.hpp"
#include <iostream>
#include "config.hpp"
#include "helper.hpp"

const int freq = 1;

void initDataBuffer() {
    std::string value;
    std::getline(std::cin, value);
    Sender::setDataBuffer(value);
}

int main() {
    initDataBuffer();

    B15F& drv = B15F::getInstance();

    Helper::setChannel(Config::CHANNEL_A, Config::a_isWrite, drv);
    Helper::setChannel(Config::CHANNEL_B, Config::b_isWrite, drv);

    Sender::reset(drv, Config::CHANNEL_A);
    // drv.delay_ms(2000);

    while(1) {
        if (!Config::a_isWrite) {
            Reader::read(drv, Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(drv, Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);

        // write
        if (Config::a_isWrite) {
            Sender::send(drv, Config::CHANNEL_A, Config::a_primarySend);
        }
        if (Config::b_isWrite) {
            Sender::send(drv, Config::CHANNEL_B, !Config::a_primarySend);
        }

        //read
        if (!Config::a_isWrite) {
            Reader::read(drv, Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(drv, Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);
    }
}
