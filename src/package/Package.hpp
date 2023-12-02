#ifndef IRIS_PACKAGE_HPP
#define IRIS_PACKAGE_HPP

#include "../controlChars/controlCharDef.hpp"
#include "../config/config.hpp"

class Package {
public:
    static unsigned int package(char *buffer, unsigned int size);
    static bool hasNext();
    static char next();
private:
    static char packageBuffer[config::bufferSize * 2];
    static unsigned int packageBufferIndex;
    static unsigned int packageBufferSize;
    static bool didSendStart;
    static bool didSendEnd;
};


#endif //IRIS_PACKAGE_HPP
