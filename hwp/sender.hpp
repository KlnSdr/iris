#include <b15f/b15f.h>
#include <iostream>
#include <string> 
#include <array>
#include <vector>
#include <string>
#include "helper.hpp"

class Sender {
    public:
        static void send(B15F& drv);
        static void reset(B15F& drv);
        static void setDataBuffer(std::string newData);
    private:
        static void sendNibble(char value, B15F& drv);
        static void preprocess();
        static const char ESC;
        static const char BEGIN;
        static const char END;
        static const char ESC2;
        // static char let[11];
        static int index;
        static char lastNibble;
        static std::vector<char> data;
        static std::string rawData;
        static int checkSumme;
};
