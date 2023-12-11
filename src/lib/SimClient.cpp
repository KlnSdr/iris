#include "SimClient.hpp"

int SimClient::client_fd = -1;
bool SimClient::isConnected = false;

/**
 * @brief Establishes a connection to the simulation server.
 *
 * This method creates a UNIX domain socket and attempts to connect it to the simulation server.
 * The path to the server's socket file is "/tmp/rainbow.sock".
 * If the socket creation or connection attempt fails, an error message is printed to the standard error output and the method returns.
 * If the connection is successful, a message is printed to the standard output and the isConnected and client_fd static members of the SimClient class are updated.
 *
 * @note This method uses the perror function to print error messages. This function produces messages that are based on the global errno variable,
 * which may be changed by system calls and some library functions.
 */
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

/**
 * @brief Writes the values to the specified pins on the simulation server.
 *
 * This method constructs a command string and sends it to the simulation server to write the specified values to the specified pins.
 * The command string is constructed by appending the hexadecimal representation of each pin and its corresponding value to the string "write ".
 * The command string is then sent to the server using the send function.
 * The server's response is read into a buffer and checked for the acknowledgement string "ack".
 * If the acknowledgement is received, the method returns true. Otherwise, it returns false.
 *
 * @param pins A vector of ints representing the pins to write to. Each int should be the hexadecimal representation of a pin.
 * @param values A vector of bools representing the values to write to the pins. Each bool should represent the value to write to the corresponding pin in the pins vector.
 * @return A bool indicating whether the write operation was acknowledged by the server. Returns true if the operation was acknowledged, false otherwise.
 *
 * @note This method checks if the size of the pins and values vectors are equal. If they are not, an error message is logged and the method returns false.
 * @note This method checks if the client is connected to the server before attempting to send the command. If the client is not connected, the method returns false.
 * @note This method uses the perror function to print error messages. This function produces messages that are based on the global errno variable,
 * which may be changed by system calls and some library functions.
 */
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

/**
 * @brief Reads the values from the specified pins on the simulation server.
 *
 * This method constructs a command string and sends it to the simulation server to read the values from the specified pins.
 * The command string is constructed by appending the hexadecimal representation of each pin to the string "read ".
 * The command string is then sent to the server using the send function.
 * The server's response is read into a buffer and parsed. If the response starts with "values", the values of the pins are extracted from the response and added to the result vector.
 * If the response does not start with "values", an error message is logged.
 *
 * @param pins A vector of ints representing the pins to read from. Each int should be the hexadecimal representation of a pin.
 * @return A vector of bools representing the values read from the pins. Each bool represents the value read from the corresponding pin in the pins vector.
 *
 * @note This method checks if the client is connected to the server before attempting to send the command. If the client is not connected, an empty vector is returned.
 * @note This method uses the perror function to print error messages. This function produces messages that are based on the global errno variable,
 * which may be changed by system calls and some library functions.
 */
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
