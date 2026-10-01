#include "command_executor.hpp"
#include "command_parser.hpp"
#include "safety_engine.hpp"
#include "voice_engine.hpp"

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
    std::cout << "Type 'voice' to use voice commands.\n";
    std::cout << "Type 'exit' to close VoiceDesk.\n\n";

    CommandParser parser;
    CommandExecutor executor;
    SafetyEngine safety;

    const std::string modelPath =
        "third_party/whisper.cpp/models/ggml-base.en.bin";

    VoiceEngine voiceEngine(modelPath);

    bool voiceReady = voiceEngine.initialize();

    if (!voiceReady)
    {
        std::cout
            << "VoiceDesk: Voice subsystem unavailable.\n"
            << "Keyboard commands remain available.\n\n";
    }

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

        if (input == "voice" ||
            input == "listen" ||
            input == "voice command")
        {
            if (!voiceReady)
            {
                std::cout
                    << "VoiceDesk: Voice subsystem is not ready.\n";

                continue;
            }

            const std::string recognizedText =
                voiceEngine.listen();

            if (recognizedText.empty())
            {
                std::cout
                    << "VoiceDesk: No speech recognized.\n";

                continue;
            }

            std::cout
                << "You said: "
                << recognizedText
                << "\n";

            if (!safety.isSafe(recognizedText))
            {
                std::cout
                    << "VoiceDesk Safety Engine: "
                    << "This voice command has been blocked "
                    << "for security reasons.\n";

                continue;
            }

            ParsedCommand voiceCommand =
                parser.parse(recognizedText);

            if (voiceCommand.type == CommandType::EXIT)
            {
                std::cout << "\nGoodbye!\n";
                break;
            }

            executor.execute(voiceCommand);

            continue;
        }

        if (!safety.isSafe(input))
        {
            std::cout
                << "VoiceDesk Safety Engine: "
                << "This command has been blocked "
                << "for security reasons.\n";

            continue;
        }

        ParsedCommand command =
            parser.parse(input);

        if (command.type == CommandType::EXIT)
        {
            std::cout << "\nGoodbye!\n";
            break;
        }

        executor.execute(command);
    }

    return 0;
}
