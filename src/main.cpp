#include <b15f/b15f.h>
#include "reader/Reader.hpp"
#include "sender/Sender.hpp"
#include <iostream>
#include "config/Config.hpp"
#include "helper/Helper.hpp"

const int freq = 1;

void initDataBuffer() {
    std::string value;
    std::getline(std::cin, value);
    Sender::setDataBuffer(value);
}

void printBanner() {
    std::cerr << "#### ########  ####  ######" << std::endl;
    std::cerr << " ##  ##     ##  ##  ##    ##" << std::endl;
    std::cerr << " ##  ##     ##  ##  ##" << std::endl;
    std::cerr << " ##  ########   ##   ######" << std::endl;
    std::cerr << " ##  ##   ##    ##        ##" << std::endl;
    std::cerr << " ##  ##    ##   ##  ##    ##" << std::endl;
    std::cerr << "#### ##     ## ####  ######" << std::endl;
    std::cerr << std::endl;
}

int main() {
    printBanner();
    Config::setup();
    initDataBuffer();

    B15F &drv = Connector::getInstance().getDrv();

    Helper::setChannel(Config::CHANNEL_A, Config::a_isWrite, drv);
    Helper::setChannel(Config::CHANNEL_B, Config::b_isWrite, drv);

    Sender::reset(Config::CHANNEL_A);
    // drv.delay_ms(2000);

    while (1) {
        if (!Config::a_isWrite) {
            Reader::read(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);

        // write
        if (Config::a_isWrite) {
            Sender::send(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (Config::b_isWrite) {
            Sender::send(Config::CHANNEL_B, !Config::a_primarySend);
        }

        //read
        if (!Config::a_isWrite) {
            Reader::read(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);
    }
}