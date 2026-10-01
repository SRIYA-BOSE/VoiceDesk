#include "command_executor.hpp"

#include "file_manager.hpp"
#include "process_manager.hpp"
#include "system_monitor.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
    bool isAllowedApplication(const std::string& application)
    {
        return application == "firefox" ||
               application == "xterm" ||
               application == "gedit" ||
               application == "nautilus";
    }
}

void CommandExecutor::execute(const ParsedCommand& command)
{
    switch (command.type)
    {
        case CommandType::SYSTEM_INFO:
        {
            SystemMonitor monitor;
            monitor.displaySystemInformation();
            break;
        }

        case CommandType::SHOW_PROCESSES:
        {
            ProcessManager manager;
            manager.listProcesses();
            break;
        }

        case CommandType::LIST_FILES:
        {
            FileManager manager;
            manager.listFiles(".");
            break;
        }

        case CommandType::CREATE_FILE:
        {
            FileManager manager;

            if (command.argument.empty())
            {
                std::cout << "Please specify a file name.\n";
                break;
            }

            manager.createFile(command.argument);
            break;
        }

        case CommandType::DELETE_FILE:
        {
            FileManager manager;

            if (command.argument.empty())
            {
                std::cout << "Please specify a file name.\n";
                break;
            }

            manager.deleteFile(command.argument);
            break;
        }

        case CommandType::OPEN_APPLICATION:
        {
            if (command.argument.empty())
            {
                std::cout << "Please specify an application.\n";
                break;
            }

            if (!isAllowedApplication(command.argument))
            {
                std::cout
                    << "VoiceDesk: Application is not in the allowed list: "
                    << command.argument << "\n";

                break;
            }

            std::cout
                << "VoiceDesk: Opening "
                << command.argument
                << "...\n";

            std::string commandToRun =
                command.argument + " >/dev/null 2>&1 &";

            std::system(commandToRun.c_str());

            break;
        }

        case CommandType::CLOSE_APPLICATION:
        {
            if (command.argument.empty())
            {
                std::cout << "Please specify an application.\n";
                break;
            }

            if (!isAllowedApplication(command.argument))
            {
                std::cout
                    << "VoiceDesk: Application is not in the allowed list: "
                    << command.argument << "\n";

                break;
            }

            std::cout
                << "VoiceDesk: Closing "
                << command.argument
                << "...\n";

            std::string commandToRun =
                "pkill " + command.argument;

            std::system(commandToRun.c_str());

            break;
        }

        case CommandType::HELP:
            showHelp();
            break;

        case CommandType::UNKNOWN:
            std::cout
                << "I didn't understand that command.\n"
                << "Type 'help' to see available commands.\n";
            break;

        default:
            break;
    }
}

void CommandExecutor::showHelp()
{
    std::cout << "\n";
    std::cout << "========== VoiceDesk Commands ==========\n";
    std::cout << "system info       - Show system information\n";
    std::cout << "list files        - List files\n";
    std::cout << "show processes    - Show running processes\n";
    std::cout << "create file NAME  - Create a file\n";
    std::cout << "delete file NAME  - Delete a file\n";
    std::cout << "open APP          - Open an application\n";
    std::cout << "close APP         - Close an application\n";
    std::cout << "help              - Show this help\n";
    std::cout << "exit              - Exit VoiceDesk\n";
    std::cout << "========================================\n\n";
}
