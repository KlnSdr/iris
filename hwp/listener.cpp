#include "listener.hpp"

const char Reader::esc = 0x0A;
bool Reader::escbool = false;
const char Reader::esc2 = 0x0D;
bool Reader::esc2bool = false;
int Reader::beginzaehler = 1;
const char Reader::begin = 0x05;
bool Reader::beginbool= false;
const char Reader::end = 0x07;
bool Reader::endbool = false;
int Reader::buffer = 0;
int Reader::offset = 0;
bool Reader::wasNull = true;
int Reader::pause = 0;
int Reader::compareWert = 0;
int Reader::checkSumsize = 1;
std::vector<char> Reader::dataBuffer = {};

char Reader::normalizeReading(char rawRead) {
    char normalized = 0;
    char shifted = rawRead >> 4;

    for (int i = 0; i < 4; i++) {
        char lsb = shifted & 0x1;
        lsb <<= 3 - i;
        normalized |= lsb;
        shifted >>= 1;
    }

    return normalized;
}

void Reader::calculateCheckSumAndPrint() {
    if (dataBuffer.size() < checkSumsize) {
        std::cout << "[verworfen: zu kurz]" << std::endl;
        return;
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

    if (calcedCheckSum == checkSum) {
        // std::cout << "gleich" << std::endl;
        std::cout << dataBufferString << std::endl;
    } else {
        // std::cout << "fuck" << std::endl;
        std::cout << "[verworfen]" << std::endl;
    }

    dataBuffer.clear();
    // std::cout << "======" << std::endl;
}

void Reader::read(B15F& drv) {
		
    char value = normalizeReading((char) drv.getRegister(&PINA));
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

    if (value == esc2 && compareWert == value){
        esc2bool = true;
    }

    if (value == esc && esc2bool == false) {
        escbool = true;
    }

    if(value == begin && beginbool == false){
        // std::cout << "=== begin ===" << std::endl;
        beginbool = true;
        endbool = false;
        escbool = false;
        esc2bool = false;
        return;
    }
    
    
    if(value == end && esc2bool == true){
        endbool = false;
        esc2bool = false;
        
    } else if(value == end && esc2bool == false){
        endbool = true;
        beginbool = false;
        buffer = 0;
        offset = 0;
        calculateCheckSumAndPrint();
        // std::cout << std::endl;
        // std::cout << "=== end ===" << std::endl;
    }

    if (esc2bool == true && value != esc2) {
        esc2bool = false;
    }
    
    // std::cout << "begin: " << beginbool << "  End: " << endbool << "  esc2:  " << esc2bool << "  esc:   " << escbool << std::endl;
    if (beginbool == true && endbool == false && esc2bool == false && escbool == false) {
    
        buffer <<= offset;
        // std::cout << "add to buffer: " << std::hex << (int) value << std::endl;
        buffer += value;
        offset += 4;
        wasNull = false;

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


