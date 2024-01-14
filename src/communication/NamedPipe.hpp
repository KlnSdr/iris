#ifndef IRIS_NAMEDPIPE_HPP
#define IRIS_NAMEDPIPE_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <cstring>
#include <iostream>
#include "../logger/Logger.hpp"


class NamedPipe {
public:
    static void writeMessage(const char *message);

    static ssize_t readMessage(char *buffer, size_t bufferSize);

private:
    static const char *pipeName;
};


#endif //IRIS_NAMEDPIPE_HPP
