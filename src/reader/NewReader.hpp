//
// Created by kilian on 03.01.24.
//

#ifndef IRIS_NEWREADER_HPP
#define IRIS_NEWREADER_HPP

#include "../connector/Connector.hpp"
#include <vector>

class NewReader {
public:
    static void read(int channel, bool isPrimarySend);

private:
    static char compareValue;
    static bool isESC1;
    static bool isESC2;
    static bool isESC3;
    static bool inPackage;
    static void initPackage();
    static char buffer;
    static bool isSecondNibble;
    static std::vector<char> dataBuffer;
    static void processPackage();
};


#endif //IRIS_NEWREADER_HPP
