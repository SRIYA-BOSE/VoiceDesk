#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "application_manager.hpp"
#include "browser_manager.hpp"
#include "command_parser.hpp"
#include "device_driver_client.hpp"

class CommandExecutor
{
public:
    CommandExecutor();

    void execute(
        const ParsedCommand& command
    );

private:
    void showHelp();

    ApplicationManager application_manager_;
    BrowserManager browser_manager_;
    DeviceDriverClient driver_;
};

#endif
