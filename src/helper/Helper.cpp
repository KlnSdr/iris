#include "Helper.hpp"

int Helper::calcChecksum(std::string data) {
    int checkSumme = 0;
    for(int i = 0; i < data.length(); i++){
        checkSumme += data.at(i);
    }
    return checkSumme % 0xFF;
}

void Helper::setChannel(int channel, bool isWrite, B15F &drv) {
    uint8_t value = drv.getRegister(&DDRA);
    if (isWrite) {
        drv.setRegister(&DDRA, value | channel);
    } else {
        drv.setRegister(&DDRA, value & ~channel);
    }
}

std::string Helper::charToHex(char chr) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(chr);
    std::string hexString = ss.str();
    return hexString;
}
