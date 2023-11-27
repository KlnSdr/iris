#ifndef IRIS_CONNECTOR_HPP
#define IRIS_CONNECTOR_HPP

#include <vector>


class Connector {
public:
    virtual std::vector<bool> readPins() = 0;

    virtual void writePins(std::vector<bool> values) = 0;

    virtual ~Connector() = default;
};


#endif //IRIS_CONNECTOR_HPP
