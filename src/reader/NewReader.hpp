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

    static void processControlPackage();

    static char extractChecksum();
};


#endif //IRIS_NEWREADER_HPP
