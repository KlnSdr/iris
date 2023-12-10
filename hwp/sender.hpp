#include <b15f/b15f.h>
#include <iostream>
#include <string> 
#include <array>
#include <vector>
#include <string>
#include "helper.hpp"
#include "config.hpp"

class Sender {
    public:
        static void send(B15F& drv, int channel, bool isPrimarySend);
        static void reset(B15F& drv, int channel);
        static void setDataBuffer(std::string newData);
    private:
        static void sendNibble(char value, B15F& drv, int channel);
        static void preprocess();
        static const char ESC;
        static const char BEGIN;
        static const char END;
        static const char ESC2;
        static const char OK;
        static const char RESEND;
        // static char let[11];
        static int index;
        static char lastNibble;
        static std::vector<char> data;
        static std::string rawData;
        static int checkSumme;
};
