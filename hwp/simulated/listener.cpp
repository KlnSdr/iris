#include <iostream>
#include "simClient.hpp"
#include <iomanip>
#include <unistd.h>
#include <vector>

using namespace std;

int readPins() {
    std::vector<bool> pins = SimClient::readPins({8, 9, 0xa, 0xb});
    int value = 0;
    for (int i = 0; i < pins.size(); i++) {
        value += pins[i] << i;
    }
    return value;
}

void sleepMs(int ms) {
    usleep(ms * 1000);
}

int main() {
    SimClient::connectSim();

    int buffer = 0;
    int offset = 0;
    bool wasNull = true;
    int pause = 0;
    int bigPause = 0;

    while(bigPause < 500) {
        int value = readPins();
        if (value != 0 && wasNull) {
            bigPause = 0;
            pause = 0;

            buffer <<= offset;
            buffer += value;
            offset += 4;
            wasNull = false;

            if (offset > 4) {
                // std::cout << std::hex << buffer << " => " << static_cast<char>(buffer) << std::endl;
                std::cout << static_cast<char>(buffer) << std::flush;
                offset = 0;
                buffer = 0;
            }
        } else if (value == 0 && !wasNull) {
            wasNull = true;
        } else if (value == 0 && wasNull) {
            pause++;
            bigPause++;
        }

        if (pause >= 6) {
            buffer = 0;
            offset = 0;
            pause = 0;
        }
        sleepMs(5);
    }
}
