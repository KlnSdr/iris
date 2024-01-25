#include "Config.hpp"

bool Config::gotBeacon = false;

bool Config::isRunning = true;

bool Config::didSendEOT = false;
bool Config::willSendEOT = false;
bool Config::gotEOT = false;

/**
 * @brief This method is used to setup the Config class.
 *
 * It disables the synchronization of the C and C++ standard streams and sets the log level for the Logger class.
 *
 * @note The synchronization is disabled to speed up the I/O operations.
 */
/**
 * Shirley: "Ooh they've got a class on how to write jokes."
 * Annie: "Don't take that. I dropped it after a class on setups. The professor is so old..."
 * (Community, S2E20 - Competitive Wine Tasting)
 */
void Config::setup() {
    // Disables the synchronization of the C and C++ standard streams.
    std::ios::sync_with_stdio(false);

    // Sets the log level for the Logger class.
    Logger::setLogLevel(logLevel);
}
