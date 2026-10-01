#include "command_executor.hpp"

#include "file_manager.hpp"
#include "process_manager.hpp"
#include "system_monitor.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    std::string normalizeApplicationName(
        const std::string& input)
    {
        std::string result = input;

        std::transform(
            result.begin(),
            result.end(),
            result.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(
                    std::tolower(c));
            });

        return result;
    }

    bool isAllowedApplication(
        const std::string& application)
    {
        const std::string normalized =
            normalizeApplicationName(application);

        const std::vector<std::string> allowedApplications =
        {
            "firefox",
            "xterm",
            "gedit",
            "nautilus"
        };

        return std::find(
            allowedApplications.begin(),
            allowedApplications.end(),
            normalized) != allowedApplications.end();
    }
}

CommandExecutor::CommandExecutor()
    : driver_("/dev/voicedesk")
{
    if (driver_.isOpen())
    {
        std::cout
            << "VoiceDesk: Linux device driver connected.\n";
    }
    else
    {
        std::cout
            << "VoiceDesk: Device driver unavailable.\n"
            << "VoiceDesk: Continuing in user-space mode.\n";
    }
}

void CommandExecutor::execute(
    const ParsedCommand& command)
{
    /*
     * Send the validated command to the Linux
     * character device before performing the
     * user-space action.
     *
     * The safety check is performed in main.cpp
     * before reaching this function.
     */
    if (driver_.isOpen())
    {
        std::string driverCommand;

        switch (command.type)
        {
            case CommandType::OPEN_APPLICATION:
                driverCommand =
                    "open " + command.argument;
                break;

            case CommandType::CLOSE_APPLICATION:
                driverCommand =
                    "close " + command.argument;
                break;

            case CommandType::SYSTEM_INFO:
                driverCommand =
                    "system information";
                break;

            case CommandType::SHOW_PROCESSES:
                driverCommand =
                    "show processes";
                break;

            case CommandType::LIST_FILES:
                driverCommand =
                    "list files";
                break;

            case CommandType::CREATE_FILE:
                driverCommand =
                    "create file " + command.argument;
                break;

            case CommandType::DELETE_FILE:
                driverCommand =
                    "delete file " + command.argument;
                break;

            case CommandType::HELP:
                driverCommand =
                    "help";
                break;

            default:
                break;
        }

        if (!driverCommand.empty())
        {
            if (!driver_.sendCommand(driverCommand))
            {
                std::cout
                    << "VoiceDesk: Warning - "
                    << "command could not be sent to "
                    << "the device driver.\n";
            }
        }
    }

    FileManager fileManager;
    ProcessManager processManager;
    SystemMonitor systemMonitor;

    switch (command.type)
    {
        case CommandType::SYSTEM_INFO:
        {
            systemMonitor.displaySystemInformation();
            break;
        }

        case CommandType::SHOW_PROCESSES:
        {
            processManager.listProcesses();
            break;
        }

        case CommandType::LIST_FILES:
        {
            fileManager.listFiles(".");
            break;
        }

        case CommandType::CREATE_FILE:
        {
            if (command.argument.empty())
            {
                std::cout
                    << "VoiceDesk: Please specify a filename.\n";
                break;
            }

            if (fileManager.createFile(command.argument))
            {
                std::cout
                    << "VoiceDesk: File created: "
                    << command.argument
                    << "\n";
            }
            else
            {
                std::cout
                    << "VoiceDesk: Failed to create file.\n";
            }

            break;
        }

        case CommandType::DELETE_FILE:
        {
            if (command.argument.empty())
            {
                std::cout
                    << "VoiceDesk: Please specify a filename.\n";
                break;
            }

            if (fileManager.deleteFile(command.argument))
            {
                std::cout
                    << "VoiceDesk: File deleted: "
                    << command.argument
                    << "\n";
            }
            else
            {
                std::cout
                    << "VoiceDesk: Failed to delete file.\n";
            }

            break;
        }

        case CommandType::OPEN_APPLICATION:
        {
            if (command.argument.empty())
            {
                std::cout
                    << "VoiceDesk: Please specify an application.\n";
                break;
            }

            if (!isAllowedApplication(command.argument))
            {
                std::cout
                    << "VoiceDesk: Application is not in "
                    << "the allowed list: "
                    << command.argument
                    << "\n";

                break;
            }

            const std::string application =
                normalizeApplicationName(command.argument);

            std::cout
                << "VoiceDesk: Opening "
                << application
                << "...\n";

            const std::string launchCommand =
                application +
                " >/dev/null 2>&1 &";

            std::system(launchCommand.c_str());

            break;
        }

        case CommandType::CLOSE_APPLICATION:
        {
            if (command.argument.empty())
            {
                std::cout
                    << "VoiceDesk: Please specify an application.\n";
                break;
            }

            if (!isAllowedApplication(command.argument))
            {
                std::cout
                    << "VoiceDesk: Application is not in "
                    << "the allowed list: "
                    << command.argument
                    << "\n";

                break;
            }

            const std::string application =
                normalizeApplicationName(command.argument);

            std::cout
                << "VoiceDesk: Closing "
                << application
                << "...\n";

            const std::string closeCommand =
                "pkill -x " + application;

            std::system(closeCommand.c_str());

            break;
        }

        case CommandType::HELP:
        {
            showHelp();
            break;
        }

        case CommandType::UNKNOWN:
        default:
        {
            std::cout
                << "I didn't understand that command.\n"
                << "Type 'help' to see available commands.\n";

            break;
        }
    }
}

void CommandExecutor::showHelp()
{
    std::cout << "\n";
    std::cout << "Available VoiceDesk commands:\n";
    std::cout << "  voice\n";
    std::cout << "  open firefox\n";
    std::cout << "  close firefox\n";
    std::cout << "  system information\n";
    std::cout << "  show processes\n";
    std::cout << "  list files\n";
    std::cout << "  create file <name>\n";
    std::cout << "  delete file <name>\n";
    std::cout << "  help\n";
    std::cout << "  exit\n";
    std::cout << "\n";
}
