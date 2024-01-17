#include <b15f/b15f.h>
#include "reader/NewReader.hpp"
#include "sender/Sender.hpp"
#include <iostream>
#include "config/Config.hpp"
#include "helper/Helper.hpp"
#include "connector/Connector.hpp"
#include "enums/ControlCharDef.hpp"
#include "console/ConsoleReceiver.hpp"
#include "console/Console.hpp"
#include <unistd.h>
#include <string>

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

void startInConsoleMode() {
    Logger::info("Starting in console mode");

    char testConnection[] = "testConnection";
    Console::writeCommand(testConnection);

    while (Config::isRunning) {
        std::cout << "<iris> ";

        std::string line;
        std::getline(std::cin, line);

        Console::writeCommand((char *) line.c_str());

        if (line == "exit") {
            Config::isRunning = false;
        }
    }
}

void startInActiveMode() {
    Config::setup();
    Helper::readNextBufferAndPackage();

    B15F &drv = Connector::getInstance().getDrv();

    Helper::setChannel(Config::CHANNEL_A, true, drv);   // set A to writeMessage
    Helper::setChannel(Config::CHANNEL_B, false, drv);  // set B to read

    Connector::getInstance().writeChannel(Config::CHANNEL_A, ControlChars::BEACON);

    char listenOnlyIterations = 10;

    while (Config::isRunning) {
        // readMessage from console
        ConsoleReceiver::run();
//        usleep(10000);

        // read
        NewReader::read(Config::CHANNEL_B);
        drv.delay_ms(freq);

        //read
        NewReader::read(Config::CHANNEL_B);
        drv.delay_ms(freq);

        if (listenOnlyIterations > 0) {
            listenOnlyIterations--;
            continue;
        }

        // write
        Sender::send(Config::CHANNEL_A);
        drv.delay_ms(freq);
    }
    Sender::reset(Config::CHANNEL_A);
}

void readArgsAndRun(int argc, char **argv) {
    if (argc == 1) {
        startInActiveMode();
        return;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--console") {
            startInConsoleMode();
            return;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [OPTION]" << std::endl;
            std::cout << "  -c, --console" << std::endl;
            std::cout << "  -h, --help" << std::endl;
            return;
        } else {
            std::cerr << "Unknown argument " << arg << std::endl;
        }
    }
}

// sadly not a ruust program :(
int main(int argc, char **argv) {
    printBanner();
    readArgsAndRun(argc, argv);

    std::cerr << "Bye!" << std::endl;
    return 0;
}
