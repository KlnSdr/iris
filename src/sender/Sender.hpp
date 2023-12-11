#include <b15f/b15f.h>
#include <iostream>
#include <string> 
#include <array>
#include <vector>
#include <string>
#include "../helper/Helper.hpp"
#include "../config/Config.hpp"
#include "../controlChars/ControlCharDef.hpp"

class Sender {
    public:
        static void send(B15F& drv, int channel, bool isPrimarySend);
        static void reset(B15F& drv, int channel);
        static void setDataBuffer(std::string newData);
    private:
        static void sendNibble(char value, B15F& drv, int channel);
        static void preprocess();
        // static char let[11];
        static int index;
        static char lastNibble;
        static std::vector<char> data;
        static std::string rawData;
        static int checkSumme;
};
