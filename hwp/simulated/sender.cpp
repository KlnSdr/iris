#include <iostream>
#include <string> 
#include <array>
#include "simClient.hpp"
#include <vector>
#include "unistd.h"

void sleepMs(int ms) {
    usleep(ms * 1000);
}

void writeNibble(int n) {
    SimClient::writePins(
            {0, 1, 2, 3},
            {(n & 0x1) != 0, (n & 0x2) >> 1 != 0, (n & 0x4) >> 2 != 0, (n & 0x8) >> 3 != 0}
    );
}

int main(){
    SimClient::connectSim();
    
    writeNibble(0);
    sleepMs(10);

    char let[12] = {'H', 'E', 'L', 'L', 'O', '_', 'W', 'O', 'R', 'L', 'D', '!'};

    int i= 0;

    while(i <= 11) {
        int m = (int)let[i] & 0xF0;
        int n = (int)let[i] & 0x0F;
        int o= m >> 4;
        i++;

        writeNibble(o);
        sleepMs(10);
        writeNibble(0);
        sleepMs(10);
        writeNibble(n);
        sleepMs(10);
        writeNibble(0);
        sleepMs(10);
    }

    SimClient::disconnect();
}
