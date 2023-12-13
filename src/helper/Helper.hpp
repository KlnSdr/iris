#pragma once

#include <string>
#include <b15f/b15f.h>
#include "../logger/Logger.hpp"
#include "../config/Config.hpp"
#include "../sender/Sender.hpp"
#include "../io/IO.hpp"

class Helper {
public:
    static int calcChecksum(std::string data);

    static void setChannel(int channel, bool isWrite, B15F &drv);

    static std::string charToHex(char chr);
    static void readNextBufferAndPackage();
};
