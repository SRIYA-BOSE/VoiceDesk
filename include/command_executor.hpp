#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "command_parser.hpp"
#include "device_driver_client.hpp"

class CommandExecutor
{
public:
    CommandExecutor();

    void execute(const ParsedCommand& command);

private:
    void showHelp();

    DeviceDriverClient driver_;
};

#endif
