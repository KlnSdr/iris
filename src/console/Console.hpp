#ifndef IRIS_CONSOLE_HPP
#define IRIS_CONSOLE_HPP

#include "../communication/NamedPipe.hpp"

class Console {
public:
    static void writeCommand(char *command);
};


#endif //IRIS_CONSOLE_HPP
