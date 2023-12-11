#pragma once

#include <string>
#include <b15f/b15f.h>

class Helper {
    public:
        static int calcChecksum(std::string data);
        static void setChannel(int channel, bool isWrite, B15F &drv);
        static std::string charToHex(char chr);
};
