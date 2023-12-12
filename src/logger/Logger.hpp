#ifndef IRIS_LOGGER_HPP
#define IRIS_LOGGER_HPP

#include <iostream>

class Logger {
public:
    enum class LogLevel {
        ERROR, INFO, DEBUG
    };

    static void setLogLevel(LogLevel level);

    static void debug(const std::string &message);

    static void info(const std::string &message);

    static void error(const std::string &message);

private:
    static LogLevel logLevel;

    static void log(const std::string &level, const std::string &message, LogLevel messageLevel);
};


#endif //IRIS_LOGGER_HPP
