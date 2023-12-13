#pragma once

#include <b15f/b15f.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <string>
#include "../helper/Helper.hpp"
#include "../config/Config.hpp"
#include "../controlChars/ControlCharDef.hpp"
#include "../logger/Logger.hpp"
#include "../connector/Connector.hpp"

class Sender {
public:
    static void send(int channel, bool isPrimarySend);

    static void reset(int channel);

    static void setDataBuffer(std::string newData);
    static bool disableSend;

private:

    static void preprocess();

    // static char let[11];
    static int index;
    static char lastNibble;
    static std::vector<char> data;
    static std::string rawData;
    static int checkSumme;
    static bool didSendOkResend;
};
