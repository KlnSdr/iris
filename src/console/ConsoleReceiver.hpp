#ifndef IRIS_CONSOLERECEIVER_HPP
#define IRIS_CONSOLERECEIVER_HPP

#include <unistd.h>
#include "../communication/NamedPipe.hpp"
#include "../config/Config.hpp"

class ConsoleReceiver {
public:
    static void run();

private:
    static void interpretCommand(char *command);
};


#endif //IRIS_CONSOLERECEIVER_HPP
