#include "NamedPipe.hpp"

const char *NamedPipe::pipeName = "/tmp/iris.pipe";

void NamedPipe::writeMessage(const char *message) {
    // Create the named pipe
    mkfifo(pipeName, 0666);

    // Open the pipe for writing
    int pipeDescriptor = open(pipeName, O_WRONLY);

    // Write a message to the pipe
    write(pipeDescriptor, message, strlen(message) + 1);

    // Close the pipe
    close(pipeDescriptor);
}

ssize_t NamedPipe::readMessage(char *buffer, size_t bufferSize) {
    // Open the named pipe for reading with non-blocking flag
    int pipeDescriptor = open(pipeName, O_RDONLY | O_NONBLOCK);

    // Check if the pipe is open successfully
    if (pipeDescriptor == -1) {
        return 0;
    }

    // Set up a timeval structure for the timeout in select
    struct timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

    // Set up the file descriptor set for select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(pipeDescriptor, &read_fds);

    // Use select to check if data is available
    int readyDescriptors = select(pipeDescriptor + 1, &read_fds, nullptr, nullptr, &timeout);

    ssize_t bytesRead = 0;


    if (readyDescriptors == -1) {
        bytesRead = 0;
    } else if (readyDescriptors == 0) {
        // Timeout occurred
        bytesRead = 0;
    } else {
        // Data is available, read from the pipe
        bytesRead = read(pipeDescriptor, buffer, bufferSize);

        if (bytesRead > 0) {
            // std::cout << "Message from Writer Process: " << otherBuffer << std::endl;
        } else {
            // An error occurred
        }
    }

    // Close the pipe
    close(pipeDescriptor);
    return bytesRead;
    // return 0;
}
