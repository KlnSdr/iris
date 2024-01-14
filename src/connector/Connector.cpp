#include "Connector.hpp"

/**
 * @brief Reads a value from a specified channel and normalizes it.
 *
 * This method reads a raw value from a specified channel using the driver's getRegister method.
 * It then determines the value based on the channel. If the channel is CHANNEL_A,
 * the value is the lower 4 bits of the raw value. Otherwise, the value is the upper 4 bits of the raw value.
 * The value is then normalized using the normalizeReading method before being returned.
 *
 * @param channel The channel to read from. This should be either Config::CHANNEL_A or Config::CHANNEL_B.
 * @return The normalized value read from the specified channel.
 */
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

/**
 * @brief Normalizes a raw reading from a channel.
 *
 * This method takes a raw reading from a channel and normalizes it. The normalization process involves
 * iterating over the 4 least significant bits of the raw reading. For each bit, it is shifted to the left
 * by the difference between 3 and the current iteration index. The shifted bit is then bitwise OR'd with
 * the normalized value. The raw reading is then right shifted by 1 for the next iteration. The normalized
 * value is returned after all 4 bits have been processed.
 *
 * @param rawRead The raw reading from a channel. This should be a char with the 4 least significant bits
 * representing the reading.
 * @return The normalized reading. This is a char with the 4 least significant bits representing the
 * normalized reading.
 */
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

/**
 * @brief Writes a value to a specified channel.
 *
 * This method writes a value to a specified channel using the driver's setRegister method.
 * The value is first duplicated and shifted to the left by 4 bits, then bitwise OR'd with the original value.
 * This combined value is then bitwise AND'd with the channel to ensure the value is written to the correct channel.
 * A debug log message is then written using the Logger class, indicating the channel written to and the value written.
 *
 * @param channel The channel to writeMessage to. This should be either Config::CHANNEL_A or Config::CHANNEL_B.
 * @param value The value to writeMessage to the specified channel. This should be a char with the 4 least significant bits
 * representing the value.
 */
void Connector::writeChannel(int channel, char value) {
    drv.setRegister(&PORTA, (value | (value << 4)) & channel);
    Logger::debug("writeMessage to channel " + std::to_string(channel) + ": " + Helper::charToHex(value));
}

B15F &Connector::getDrv() const {
    return drv;
}
