#include "ConsoleReceiver.hpp"

void ConsoleReceiver::run() {
    char commandBuffer[128];
    ssize_t bytesRead = NamedPipe::readMessage(commandBuffer, sizeof(commandBuffer));
    if (bytesRead > 0) {
        interpretCommand(commandBuffer);
    }
}

void ConsoleReceiver::interpretCommand(char *command) {
    if (strcmp(command, "exit") == 0) {
        Config::isRunning = false;
    } else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}
