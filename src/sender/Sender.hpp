#pragma once

#include <b15f/b15f.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <string>
#include "../helper/Helper.hpp"
#include "../config/Config.hpp"
#include "../enums/ControlCharDef.hpp"
#include "../enums/PackageTypes.hpp"
#include "../logger/Logger.hpp"
#include "../connector/Connector.hpp"
#include <queue>
#include <tuple>

class Sender {
public:
    static void send(int channel);

    static void reset(int channel);

    static void addToSendQueue(PackageType type, const std::vector<char>& newData);
    static std::vector<char> getLastDataPackagePls();

private:
    static void preprocess(PackageType type);
    static void escapeSymbol(char prevNibble, char currentNibble, std::vector<char>& dataBuffer);

    static int index;
    static char lastNibble;
    static std::vector<char> lastDataPackage;
    static std::vector<char> data;
    static std::vector<char> rawData;
    static char checkSumme;
    static std::queue<std::tuple<PackageType, std::vector<char>>> sendQueue;
};
