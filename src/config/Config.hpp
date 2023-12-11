#ifndef IRIS_CONFIG_HPP
#define IRIS_CONFIG_HPP

#include <iostream>
#include <vector>
#include "../logger/Logger.hpp"

class Config {
public:
    static void setup();

    static const int CHANNEL_A = 0x0F;
    static const int CHANNEL_B = 0xF0;

    static const bool a_primarySend = true;
    static const Logger::LogLevel logLevel = Logger::LogLevel::DEBUG;

    static bool a_isWrite;
    static bool b_isWrite;

    static bool checkSumIsFOCKINGtheSame;
    static bool everythingIsOkiDoki;
};


#endif //IRIS_CONFIG_HPP
