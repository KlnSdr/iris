#include "Console.hpp"

void Console::writeCommand(char *command) {
    NamedPipe::writeMessage(command);
}
