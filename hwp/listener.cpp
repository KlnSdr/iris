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

    // std::cout << "> " << std::hex << (int) value << std::endl;

    if (escbool == true) {
        value = (~value) & 0xF;
        // std::cout << "inv " << std::hex << (int) value << std::endl;
        escbool = false;
    }

    if(value == esc2){
        esc2bool = true;
        
    }

    if (value == esc) {
        escbool = true;
    }

    if(value == begin && beginbool == false){
        beginbool = true;
        endbool = false;
        return;
    }
    
    
    if(value == end && esc2bool == true){
        endbool = false;
        esc2bool = false;
        
    } else if(value == end && esc2bool == false){
        endbool = true;
        beginbool = false;
        std::cout << std::endl;
    }
    
    if (beginbool == true && endbool == false && esc2bool == false && escbool == false) {
    
        buffer <<= offset;
        buffer += value;
        offset += 4;
        wasNull = false;

        if (offset > 4){
            std::cout << static_cast<char>(buffer) << std::flush;
            // std::cout << "+ " << std::hex << (int) buffer << " -> " << static_cast<char>(buffer) << std::endl;
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


