#include <b15f/b15f.h>
#include "reader/Reader.hpp"
#include "sender/Sender.hpp"
#include <iostream>
#include "config/Config.hpp"
#include "helper/Helper.hpp"
#include "io/IO.hpp"

const int freq = 1;

/**
 * @brief Initializes the data buffer for transmission.
 *
 * This method reads data from the IO buffer into a local buffer of size Config::bufferSize.
 * The number of bytes read is stored in the variable 'written'.
 * A string 'value' is then created from the local buffer, starting from the beginning of the buffer and ending at the position determined by subtracting 'written' from Config::bufferSize.
 * This string 'value' is then set as the data buffer in the Sender class using the setDataBuffer method.
 */
void initDataBuffer() {
    char buffer[Config::bufferSize];
    unsigned int written = IO::readBuffer(buffer, Config::bufferSize);

    std::string value = std::string(buffer, Config::bufferSize - written);
    Sender::setDataBuffer(value);
}

/**
 * @brief Prints a banner to the standard error output.
 *
 * This method prints a banner to the standard error output using the std::cerr stream.
 * The banner is a series of strings, each representing a line of the banner.
 * Each string is followed by a std::endl manipulator to insert a newline character and flush the stream.
 */
void printBanner() {
    std::cerr << "#### ########  ####  ######" << std::endl;
    std::cerr << " ##  ##     ##  ##  ##    ##" << std::endl;
    std::cerr << " ##  ##     ##  ##  ##" << std::endl;
    std::cerr << " ##  ########   ##   ######" << std::endl;
    std::cerr << " ##  ##   ##    ##        ##" << std::endl;
    std::cerr << " ##  ##    ##   ##  ##    ##" << std::endl;
    std::cerr << "#### ##     ## ####  ######" << std::endl;
    std::cerr << std::endl;
}

/**
 * @brief The main function of the program.
 *
 * This function is the entry point of the program. It performs the following steps:
 * 1. Prints a banner to the standard error output.
 * 2. Calls the setup method of the Config class to initialize the configuration.
 * 3. Initializes the data buffer for transmission.
 * 4. Retrieves a reference to the B15F driver object from the Connector class.
 * 5. Sets the direction of channels A and B using the Helper class's setChannel method.
 * 6. Resets channel A using the Sender class's reset method.
 * 7. Enters an infinite loop where it performs the following steps:
 *    a. If channel A is not set to write mode, it reads data from channel A using the Reader class's read method.
 *    b. If channel B is not set to write mode, it reads data from channel B using the Reader class's read method.
 *    c. If channel A is set to write mode, it sends data over channel A using the Sender class's send method.
 *    d. If channel B is set to write mode, it sends data over channel B using the Sender class's send method.
 *    e. If channel A is not set to write mode, it reads data from channel A using the Reader class's read method.
 *    f. If channel B is not set to write mode, it reads data from channel B using the Reader class's read method.
 *
 * @return int This function returns 0 upon successful execution.
 */
int main() {
    printBanner();
    Config::setup();
    initDataBuffer();

    B15F &drv = Connector::getInstance().getDrv();

    Helper::setChannel(Config::CHANNEL_A, Config::a_isWrite, drv);
    Helper::setChannel(Config::CHANNEL_B, Config::b_isWrite, drv);

    Sender::reset(Config::CHANNEL_A);
    // drv.delay_ms(2000);

    while (1) {
        if (!Config::a_isWrite) {
            Reader::read(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);

        // write
        if (Config::a_isWrite) {
            Sender::send(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (Config::b_isWrite) {
            Sender::send(Config::CHANNEL_B, !Config::a_primarySend);
        }

        //read
        if (!Config::a_isWrite) {
            Reader::read(Config::CHANNEL_A, Config::a_primarySend);
        }
        if (!Config::b_isWrite) {
            Reader::read(Config::CHANNEL_B, !Config::a_primarySend);
        }
        // drv.delay_ms(freq);
    }
}