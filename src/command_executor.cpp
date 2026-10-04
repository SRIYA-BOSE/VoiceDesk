#include "command_executor.hpp"

#include "file_manager.hpp"
#include "process_manager.hpp"
#include "system_monitor.hpp"

#include <iostream>

CommandExecutor::CommandExecutor()
    : application_manager_(),
      browser_manager_(),
      driver_("/dev/voicedesk")
{
}

void CommandExecutor::execute(
    const ParsedCommand& command
)
{
    switch (command.type)
    {
        case CommandType::OPEN_APPLICATION:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "open " + command.argument
                );
            }

            if (
                application_manager_.launch(
                    command.argument
                )
            )
            {
                std::cout
                    << "[VoiceDesk] Opened: "
                    << command.argument
                    << '\n';
            }
            else
            {
                std::cout
                    << "[VoiceDesk] Application not found: "
                    << command.argument
                    << '\n';
            }

            break;
        }

        case CommandType::OPEN_URL:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "open url " +
                    command.argument
                );
            }

            if (
                browser_manager_.open(
                    command.argument
                )
            )
            {
                std::cout
                    << "[VoiceDesk] Opened URL: "
                    << command.argument
                    << '\n';
            }
            else
            {
                std::cout
                    << "[VoiceDesk] Unable to open URL\n";
            }

            break;
        }

        case CommandType::SEARCH_WEB:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "search " +
                    command.argument
                );
            }

            if (
                browser_manager_.searchGoogle(
                    command.argument
                )
            )
            {
                std::cout
                    << "[VoiceDesk] Searching Google for: "
                    << command.argument
                    << '\n';
            }
            else
            {
                std::cout
                    << "[VoiceDesk] Search failed\n";
            }

            break;
        }

        case CommandType::CLOSE_APPLICATION:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "close " +
                    command.argument
                );
            }

            if (
                application_manager_.close(
                    command.argument
                )
            )
            {
                std::cout
                    << "[VoiceDesk] Closed: "
                    << command.argument
                    << '\n';
            }
            else
            {
                std::cout
                    << "[VoiceDesk] Could not close: "
                    << command.argument
                    << '\n';
            }

            break;
        }

        case CommandType::SYSTEM_INFO:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "system information"
                );
            }

            /*
             * Use the existing SystemMonitor API.
             *
             * Your current class does not have
             * showSystemInfo(), so use the
             * existing display method.
             */
            SystemMonitor monitor;
            monitor.displaySystemInformation();

            break;
        }

        case CommandType::SHOW_PROCESSES:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "show processes"
                );
            }

            ProcessManager manager;
            manager.listProcesses();

            break;
        }

        case CommandType::LIST_FILES:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "list files"
                );
            }

            FileManager manager;

            /*
             * Your existing FileManager requires
             * a path argument.
             *
             * "." means current directory.
             */
            manager.listFiles(".");

            break;
        }

        case CommandType::CREATE_FILE:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "create file " + command.argument
                );
            }

            FileManager manager;
            manager.createFile(command.argument);

            break;
        }

        case CommandType::DELETE_FILE:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "delete file " + command.argument
                );
            }

            FileManager manager;
            manager.deleteFile(command.argument);

            break;
        }


        case CommandType::DISK_INFO:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "disk information"
                );
            }

            std::cout
                << "[VoiceDesk] Disk information:\n";

            std::system("df -h");

            break;
        }

        case CommandType::NETWORK_INFO:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand(
                    "network information"
                );
            }

            std::cout
                << "[VoiceDesk] Network information:\n";

            std::system("ip addr");

            break;
        }

        case CommandType::HELP:
        {
            showHelp();
            break;
        }

        case CommandType::EXIT:
        {
            if (driver_.isOpen())
            {
                driver_.sendCommand("exit");
            }

            std::cout
                << "[VoiceDesk] Exiting...\n";

            break;
        }

        default:
        {
            std::cout
                << "[VoiceDesk] Unknown command: "
                << command.argument
                << '\n';

            break;
        }
    }
}

void CommandExecutor::showHelp()
{
    std::cout << R"(
================ VoiceDesk Commands ================

APPLICATIONS

  open firefox
  open vscode
  open terminal
  open vlc
  open file manager
  close firefox

WEB

  open youtube
  open whatsapp
  open gmail
  open github
  open google
  open linkedin
  open reddit
  open chatgpt

URL

  open https://example.com

SEARCH

  search google for Linux device drivers
  search for C++ tutorials

SYSTEM

  show system information
  show processes
  list files
  show disk space
  show network information

FILES

  create file test.txt
  delete file test.txt

ASSISTANT

  help
  exit

=====================================================
)";
}
