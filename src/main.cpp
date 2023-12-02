#include "config/config.hpp"
#include "reader/Reader.hpp"
#include "package/Package.hpp"

int main() {
    config::setup();

    bool running = true; // just for development, program will run continuously when also listening for data

    while (running) {
        if (Package::hasNext()) {
            char tmp = Package::next();
            std::cout << std::hex << (int)tmp << std::endl;
        } else {
            running = Reader::read();
            std::cout << std::endl;
        }
    }

    return 0;
}
