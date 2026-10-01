#include "command_executor.hpp"
#include "command_parser.hpp"
#include "safety_engine.hpp"

#include <iostream>
#include <string>

int main()
{
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "              VOICEDESK\n";
    std::cout << "       Linux Desktop Assistant\n";
    std::cout << "============================================\n";
    std::cout << "Type 'help' to see available commands.\n";
    std::cout << "Type 'exit' to close VoiceDesk.\n\n";

    CommandParser parser;
    CommandExecutor executor;
    SafetyEngine safety;

    while (true)
    {
        std::cout << "VoiceDesk > ";

        std::string input;

        if (!std::getline(std::cin, input))
        {
            break;
        }

        if (input.empty())
        {
            continue;
        }

        // Security validation before processing the command
        if (!safety.isSafe(input))
        {
            std::cout
                << "VoiceDesk Safety Engine: "
                << "This command has been blocked for security reasons.\n";

            continue;
        }

        ParsedCommand command = parser.parse(input);

        if (command.type == CommandType::EXIT)
        {
            std::cout << "\nGoodbye!\n";
            break;
        }

        executor.execute(command);
    }

    return 0;
}
