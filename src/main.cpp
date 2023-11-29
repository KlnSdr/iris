#include "io/io.hpp"
#include "config/config.hpp"
#include "controlChars/controlCharDef.hpp"

int main() {
    config::setup();
    char buffer[config::bufferSize];

    while (io::hasData()) {
        unsigned int sizeLeft = io::readBuffer(buffer, config::bufferSize);
        std::cout.write(buffer, config::bufferSize - sizeLeft);
    }

    enum control obj = resend;
    enum control obj1 = packageStart;
    enum control obj3 = resend;
    std::cout << std::hex << obj1 + 1 << std::endl;
    std::cout << std::hex << obj3 + 1 << std::endl;

    std::cout << std::hex << obj + 1 << std::endl;



    return 0;
}
