#include "SimClient.hpp"

int SimClient::client_fd = -1;
bool SimClient::isConnected = false;

void SimClient::connectSim() {
    const char *socket_path = "/tmp/rainbow.sock";
    int local_client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (local_client_fd == -1) {
        perror("Error creating socket");
        return;
    }

    sockaddr_un server_address;
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, socket_path, sizeof(server_address.sun_path) - 1);

    // Connect to the server
    if (connect(local_client_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to socket");
        close(local_client_fd);
        return;
    }

    std::cout << "Connected to the server.\n";

    SimClient::isConnected = true;
    SimClient::client_fd = local_client_fd;
}

bool SimClient::writePins(const std::vector<int> &pins, const std::vector<bool> &values) {

    if (!SimClient::isConnected) {
        return false;
    }

    if (pins.size() != values.size()) {
        Logger::error("pins and values must be the same size.");
        return false;
    }

    std::string command = "write ";

    for (size_t i = 0; i < pins.size(); ++i) {
        std::stringstream stream;
        stream << std::hex << pins[i];
        command += stream.str() + (values[i] ? "1" : "0") + ",";
    }

    command.pop_back();

    send(SimClient::client_fd, command.c_str(), command.size(), 0);

    char buffer[1024];
    int bytes_read = recv(SimClient::client_fd, buffer, sizeof(buffer), 0);

    if (bytes_read == -1) {
        perror("Error receiving data");
        SimClient::isConnected = false;
        close(SimClient::client_fd);
        return false;
    } else if (bytes_read == 0) {
        Logger::info("Server closed the connection.");
        SimClient::isConnected = false;
        return false;
    }

    buffer[bytes_read] = '\0';

    return strncmp(buffer, "ack", 3) == 0;
}

std::vector<bool> SimClient::readPins(const std::vector<int> &pins) {
    std::vector<bool> result;

    if (!SimClient::isConnected) {
        return result;
    }

    std::string command = "read ";

    for (int pin: pins) {
        std::stringstream stream;
        stream << std::hex << pin;
        command += stream.str() + ",";
    }

    command.pop_back();

    send(SimClient::client_fd, command.c_str(), command.size(), 0);

    char buffer[1024];
    int bytes_read = recv(SimClient::client_fd, buffer, sizeof(buffer), 0);

    if (bytes_read == -1) {
        perror("Error receiving data");
        SimClient::isConnected = false;
        close(SimClient::client_fd);
        return result;
    } else if (bytes_read == 0) {
        Logger::info("Server closed the connection.");
        SimClient::isConnected = false;
        return result;
    }

    buffer[bytes_read] = '\0';

    if (strncmp(buffer, "values", 6) == 0) {
        int pinValue;
        std::istringstream iss(buffer + 7);
        while (iss >> pinValue) {
            result.push_back(pinValue == 1);
            if (iss.peek() == ',') {
                iss.ignore();
            }
        }
    } else {
        Logger::error("Unknown command: " + std::string(buffer));
    }
    return result;
}

void SimClient::disconnect() {
    close(SimClient::client_fd);
}

bool SimClient::isConnectedToSim() {
    return SimClient::isConnected;
}
