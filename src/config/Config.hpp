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

    static const int bufferSize = 10;

    static const bool a_primarySend = true;
    static const Logger::LogLevel logLevel = Logger::LogLevel::INFO;

    static bool a_isWrite;
    static bool b_isWrite;

    static bool doPhysicalSwitch;
    static bool aIsSendChannel;

    static bool checkSumIsFOCKINGtheSame;
    static bool doSendResponse;
};


#endif //IRIS_CONFIG_HPP
