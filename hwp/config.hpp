#pragma once

class Config {
public:
    static const int CHANNEL_A = 0x0F;
    static const int CHANNEL_B = 0xF0;
    
    static const bool a_primarySend = true;

    static bool a_isWrite;
    static bool b_isWrite;

    static bool checkSumIsFOCKINGtheSame;
    static bool everythingIsOkiDoki;
};
