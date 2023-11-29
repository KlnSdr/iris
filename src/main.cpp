#include "config/config.hpp"
#include "reader/Reader.hpp"

int main() {
    config::setup();
    Reader::read();
    return 0;
}
