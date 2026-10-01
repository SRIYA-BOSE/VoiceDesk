#include "device_driver_client.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

DeviceDriverClient::DeviceDriverClient(const std::string& device_path)
    : fd_(-1),
      device_path_(device_path)
{
    fd_ = open(device_path_.c_str(), O_RDWR);

    if (fd_ < 0) {
        std::cerr
            << "[DeviceDriver] Failed to open "
            << device_path_
            << ": "
            << std::strerror(errno)
            << '\n';
        return;
    }

    std::cout
        << "[DeviceDriver] Connected to "
        << device_path_
        << '\n';
}

DeviceDriverClient::~DeviceDriverClient()
{
    if (fd_ >= 0) {
        close(fd_);

        std::cout
            << "[DeviceDriver] Driver connection closed\n";
    }
}

bool DeviceDriverClient::isOpen() const
{
    return fd_ >= 0;
}

bool DeviceDriverClient::sendCommand(const std::string& command)
{
    if (fd_ < 0) {
        std::cerr
            << "[DeviceDriver] Device is not open\n";
        return false;
    }

    const ssize_t bytes_written = write(
        fd_,
        command.c_str(),
        command.size()
    );

    if (bytes_written < 0) {
        std::cerr
            << "[DeviceDriver] Write failed: "
            << std::strerror(errno)
            << '\n';

        return false;
    }

    if (static_cast<std::size_t>(bytes_written) != command.size()) {
        std::cerr
            << "[DeviceDriver] Partial write\n";

        return false;
    }

    std::cout
        << "[DeviceDriver] Command sent: "
        << command
        << '\n';

    return true;
}

std::string DeviceDriverClient::readCommand()
{
    if (fd_ < 0) {
        return {};
    }

    char buffer[256] = {};

    const ssize_t bytes_read = read(
        fd_,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytes_read < 0) {
        std::cerr
            << "[DeviceDriver] Read failed: "
            << std::strerror(errno)
            << '\n';

        return {};
    }

    return std::string(buffer, static_cast<std::size_t>(bytes_read));
}
