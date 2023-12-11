#ifndef IRIS_CONNECTOR_HPP
#define IRIS_CONNECTOR_HPP

#include <vector>


class Connector {
public:
    virtual char readPins() = 0;

    virtual void writePins(char value) = 0;

    virtual void wait() = 0;

    virtual ~Connector() = default;
};


#endif //IRIS_CONNECTOR_HPP
