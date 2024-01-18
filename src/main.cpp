#include <b15f/b15f.h>
#include "reader/NewReader.hpp"
#include "sender/Sender.hpp"
#include <iostream>
#include "config/Config.hpp"
#include "helper/Helper.hpp"

const int freq = 1;

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
 // sadly not a ruust program :(
int main() {
    printBanner();
    Config::setup();
    Helper::readNextBufferAndPackage();

    B15F &drv = Connector::getInstance().getDrv();

    Helper::setChannel(Config::CHANNEL_A, true, drv);   // set A to write
    Helper::setChannel(Config::CHANNEL_B, false, drv);  // set B to read

    Sender::reset(Config::CHANNEL_A);

    while (1) {
        // read
        NewReader::read(Config::CHANNEL_B);
        drv.delay_ms(freq);

        // write
        Sender::send(Config::CHANNEL_A);
        drv.delay_ms(freq);

        //read
        NewReader::read(Config::CHANNEL_B);
        drv.delay_ms(freq);
    }
}
