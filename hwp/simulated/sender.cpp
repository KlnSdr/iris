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
    sleepMs(20);

    char let[5] = {'A', 'B', 'C','D','E'};

    int i= 0;

    while(i <= 4) {
        int m = (int)let[i] & 0xF0;
        int n = (int)let[i] & 0x0F;
        int o= m >> 4;
        i++;

        writeNibble(o);
        sleepMs(20);
        writeNibble(0);
        sleepMs(20);
        writeNibble(n);
        sleepMs(20);
        writeNibble(0);
        sleepMs(40);
    }

    SimClient::disconnect();
}
