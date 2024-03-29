#include "Logger.hpp"

Logger::LogLevel Logger::logLevel = LogLevel::INFO;

void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}

void Logger::debug(const std::string &message) {
    log("DEBUG", message, LogLevel::DEBUG);
}

void Logger::info(const std::string &message) {
    log("INFO", message, LogLevel::INFO);
}

void Logger::log(const std::string &level, const std::string &message, LogLevel messageLevel) {
    if (messageLevel > logLevel) {
        return;
    }

    std::cerr << "[" << level << "] " << message << std::endl;
}

void Logger::error(const std::string &message) {
    log("ERROR", message, LogLevel::ERROR);
}
