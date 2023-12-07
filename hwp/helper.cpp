#include "helper.hpp"

int Helper::calcChecksum(std::string data) {
    int checkSumme = 0;
    for(int i = 0; i < data.length(); i++){
        checkSumme += data.at(i);
    }
    return checkSumme % 0xFF;
}
