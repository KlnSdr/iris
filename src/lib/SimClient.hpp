#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include "../logger/Logger.hpp"

class SimClient {
public:
    static std::vector<bool> readPins(const std::vector<int> &pins);

    static bool writePins(const std::vector<int> &pins, const std::vector<bool> &values);

    static void connectSim();

    static void disconnect();

    static bool isConnectedToSim();

private:
    static bool isConnected;
    static int client_fd;
};
