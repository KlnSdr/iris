#include "Connector.hpp"

char Connector::readChannel(int channel) {
    char rawValue = (char) drv.getRegister(&PINA);
    char value;

    if (channel == Config::CHANNEL_A) {
        value = rawValue & 0xF;
    } else {
        value = rawValue >> 4;
    }
    return normalizeReading(value);
}

char Connector::normalizeReading(char rawRead) {
    char normalized = 0;

    for (int i = 0; i < 4; i++) {
        char lsb = rawRead & 0x1;
        lsb <<= 3 - i;
        normalized |= lsb;
        rawRead >>= 1;
    }

    return normalized;
}

Connector::Connector(B15F &drv) : drv(drv) {
}

Connector Connector::getInstance() {
    static Connector instance(B15F::getInstance());
    return instance;
}

void Connector::writeChannel(int channel, char value) {
    drv.setRegister(&PORTA, (value | (value << 4)) & channel);
    Logger::debug("write to channel " + std::to_string(channel) + ": " + Helper::charToHex(value));
}

B15F &Connector::getDrv() const {
    return drv;
}
