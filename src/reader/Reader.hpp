#ifndef IRIS_READER_HPP
#define IRIS_READER_HPP

#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../package/Package.hpp"

class Reader {
public:
    static bool read();
private:
    static char mainBuffer[config::bufferSize];
    static unsigned int mainBufferOffset;
};


#endif //IRIS_READER_HPP
