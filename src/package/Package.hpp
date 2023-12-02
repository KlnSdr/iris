#ifndef IRIS_PACKAGE_HPP
#define IRIS_PACKAGE_HPP

#include "../controlChars/controlCharDef.hpp"
#include "../config/config.hpp"
#include <iostream>
#include <iomanip>

class Package {
public:
    static unsigned int package(char *buffer, unsigned int size);
};


#endif //IRIS_PACKAGE_HPP
