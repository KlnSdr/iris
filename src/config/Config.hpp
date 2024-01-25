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

    static const int bufferSize = 256;

    static const Logger::LogLevel logLevel = Logger::LogLevel::INFO;
    static bool gotBeacon;

    static bool isRunning;
    static bool didSendEOT;
    static bool willSendEOT;
    static bool gotEOT;
};


#endif //IRIS_CONFIG_HPP
