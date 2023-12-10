#include "helper.hpp"

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
