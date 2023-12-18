#ifndef IRIS_IO_HPP
#define IRIS_IO_HPP

#include<iostream>

class IO {
public:
    static bool hasData();

    static unsigned int readBuffer(char *buffer, unsigned int size);

private:
    static char readByte();
    static bool _hasData;
};


#endif //IRIS_IO_HPP
