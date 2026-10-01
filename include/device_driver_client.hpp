#ifndef VOICEDESK_DEVICE_DRIVER_CLIENT_HPP
#define VOICEDESK_DEVICE_DRIVER_CLIENT_HPP

#include <string>

class DeviceDriverClient {
public:
    explicit DeviceDriverClient(
        const std::string& device_path = "/dev/voicedesk"
    );

    ~DeviceDriverClient();

    bool isOpen() const;

    bool sendCommand(const std::string& command);

    std::string readCommand();

private:
    int fd_;
    std::string device_path_;
};

#endif
