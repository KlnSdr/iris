#ifndef IRIS_IO_HPP
#define IRIS_IO_HPP

#include<iostream>

class io {
public:
    static bool hasData();

    static unsigned int readBuffer(char *buffer, unsigned int size);

private:
    static char readByte();
};


#endif //IRIS_IO_HPP
