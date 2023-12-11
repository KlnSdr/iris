#ifndef IRIS_CONFIG_HPP
#define IRIS_CONFIG_HPP

#include <iostream>
#include <vector>

class config {
public:
    static const int bufferSize = 256;
    static const std::vector<int> Rx;
    static const std::vector<int> Tx;

    static void setup();
};


#endif //IRIS_CONFIG_HPP
