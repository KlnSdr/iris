#ifndef IRIS_CONNECTOR_HPP
#define IRIS_CONNECTOR_HPP

#include <vector>
#include <b15f/b15f.h>
#include "../config/Config.hpp"
#include "../helper/Helper.hpp"


class Connector {
public:
    char readChannel(int channel);

    void writeChannel(int channel, char value);

    static Connector getInstance();

    [[nodiscard]] B15F &getDrv() const;

private:
    explicit Connector(B15F &drv);

    static char normalizeReading(char rawRead);

    B15F &drv;
    static Connector instance;
};


#endif //IRIS_CONNECTOR_HPP
