#include "Reader.hpp"

bool Reader::escbool = false;
bool Reader::esc2bool = false;
bool Reader::beginbool= false;
bool Reader::endbool = false;
int Reader::buffer = 0;
int Reader::offset = 0;
int Reader::pause = 0;
int Reader::compareWert = 0;
int Reader::checkSumsize = 1;
std::vector<char> Reader::dataBuffer = {};

char Reader::normalizeReading(char rawRead) {
    char normalized = 0;

    for (int i = 0; i < 4; i++) {
        char lsb = rawRead & 0x1;
        lsb <<= 3 - i;
        normalized |= lsb;
        rawRead >>= 1;
    }

    return normalized;
}

bool Reader::calculateCheckSumAndPrint() {
    if (dataBuffer.size() < checkSumsize) {
        std::cout << "[verworfen: zu kurz]" << std::endl;
        return false;
    }
    // std::cout << "======" << std::endl;
    // for (int i = 0; i < dataBuffer.size(); i++) {
        // std::cout << std::hex << dataBuffer.at(i) << std::endl;
    // }
    // std::cout << "-" << std::endl;
    int checkSum = 0;

    for (int i = 0; i < checkSumsize; i++) {
        int checksumByte = dataBuffer.at(dataBuffer.size() - i - 1) & 0xFF;
        checkSum += checksumByte << i;
        // std::cout << std::hex << checksumByte;
    }
    // std::cout << std::endl;
    dataBuffer.resize(dataBuffer.size() - checkSumsize);

    std::string dataBufferString(dataBuffer.begin(), dataBuffer.end());
    int calcedCheckSum = Helper::calcChecksum(dataBufferString);

    // std::cout << "calced " << calcedCheckSum << " == " << "read " << checkSum << "?" << std::endl;
    
    bool isValidPackage = calcedCheckSum == checkSum;

    if (isValidPackage) {
        // std::cout << "gleich" << std::endl;
        std::cout << dataBufferString << std::endl;
    } else {
        // std::cout << "fuck" << std::endl;
        std::cout << "[verworfen]" << std::endl;
    }

    dataBuffer.clear();
    // std::cout << "======" << std::endl;
    return isValidPackage;
}

void Reader::read(B15F& drv, int channel, bool isPrimarySend) {
    char rawValue = (char) drv.getRegister(&PINA);
    char value;

    if (channel == Config::CHANNEL_A) {
        value = rawValue & 0xF;
    } else {
        value = rawValue >> 4;
    }

    value = normalizeReading(value);

    if (isPrimarySend) {
        if (value == ControlChars::OK || value == ControlChars::RESEND) {
            Config::everythingIsOkiDoki = value == ControlChars::OK;
            
            if (channel == Config::CHANNEL_A) {
                Config::a_isWrite = true;
                Helper::setChannel(channel, true, drv);
            } else {
                Config::b_isWrite = true;
                Helper::setChannel(channel, true, drv);
            }
        }
        // todo timer falls lange kein richtiges zeichen anliegt

        return;
    }

    if (compareWert == value){
        pause++;
        return;
    } else if (pause > 20) {
        buffer = 0;
        endbool = true;
        beginbool = false;
        std::cout << std::endl;
	pause = 0;
        return;
    }
    pause = 0;
    compareWert = value;

     // std::cout << "gelesener Hex: " << std::hex << (int) value << std::endl;

    if (escbool == true) {
        value = (~value) & 0xF;
        // std::cout << "inv " << std::hex << (int) value << std::endl;
        escbool = false;
    }

    if (value == ControlChars::ESC2 && compareWert == value){
        esc2bool = true;
    }

    if (value == ControlChars::ESC1 && esc2bool == false) {
        escbool = true;
    }

    if(value == ControlChars::PCK_START && beginbool == false){
        // std::cout << "=== begin ===" << std::endl;
        beginbool = true;
        endbool = false;
        escbool = false;
        esc2bool = false;
        return;
    }
    
    
    if(value == ControlChars::PCK_END && esc2bool == true){
        endbool = false;
        esc2bool = false;
        
    } else if(value == ControlChars::PCK_END && esc2bool == false){
        endbool = true;
        beginbool = false;
        buffer = 0;
        offset = 0;

        bool isValidPackage = calculateCheckSumAndPrint();
        Config::checkSumIsFOCKINGtheSame = isValidPackage;

        if (channel == Config::CHANNEL_A) {
            Config::a_isWrite = true;
            Helper::setChannel(channel, true, drv);
        } else {
            Config::b_isWrite = true;
            Helper::setChannel(channel, true, drv);
        }

        // std::cout << std::endl;
        // std::cout << "=== end ===" << std::endl;
    }

    if (esc2bool == true && value != ControlChars::ESC2) {
        esc2bool = false;
    }
    
    // std::cout << "begin: " << beginbool << "  End: " << endbool << "  esc2:  " << esc2bool << "  esc:   " << escbool << std::endl;
    if (beginbool == true && endbool == false && esc2bool == false && escbool == false) {
    
        buffer <<= offset;
        // std::cout << "add to buffer: " << std::hex << (int) value << std::endl;
        buffer += value;
        offset += 4;

        if (offset > 4){
            // std::cout << static_cast<char>(buffer) << std::flush;
            dataBuffer.push_back(buffer);
            // std::cout << std::hex << static_cast<int>(buffer) << std::flush;
            // std::cout << "neuer Char:    " << std::hex << (int) buffer << " -> " << static_cast<char>(buffer) << std::endl;
            offset = 0;
            buffer = 0;
        }

        if (pause >= 6) {
            buffer = 0;
            offset = 0;
            pause = 0;
        }
    
    }

}


