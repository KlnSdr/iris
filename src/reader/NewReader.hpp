//
// Created by kilian on 03.01.24.
//

#ifndef IRIS_NEWREADER_HPP
#define IRIS_NEWREADER_HPP

#include "../connector/Connector.hpp"
#include "../config/Config.hpp"
#include <vector>
#include "../sender/Sender.hpp"

class NewReader {
public:
    static void read(int channel);

private:
    static char compareValue;
    static bool isESC1;
    static bool isESC2;
    static bool inPackage;
    static void initPackage();
    static char buffer;
    static bool isSecondNibble;
    static std::vector<char> dataBuffer;
    static void processPackage();
    static void processDataPackage();
    static void processResponsePackage();
    static char extractChecksum();
    static bool didValidateMessage;
};


#endif //IRIS_NEWREADER_HPP
