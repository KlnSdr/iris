#include "Config.hpp"

/**
 * @brief This method is used to setup the Config class.
 *
 * It disables the synchronization of the C and C++ standard streams and sets the log level for the Logger class.
 *
 * @note The synchronization is disabled to speed up the I/O operations.
 */
void Config::setup() {
    // Disables the synchronization of the C and C++ standard streams.
    std::ios::sync_with_stdio(false);

    // Sets the log level for the Logger class.
    Logger::setLogLevel(logLevel);
}

bool Config::a_isWrite = Config::a_primarySend;
bool Config::b_isWrite = !Config::a_primarySend;

bool Config::doPhysicalSwitch = false;
bool Config::aIsSendChannel = true;
bool Config::doSendResponse = false;
bool Config::checkSumIsFOCKINGtheSame = false;
