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
    std::cout << "Voice command mode\n";
    std::cout << "Speak ONE command and VoiceDesk will execute it.\n\n";

    CommandParser parser;
    CommandExecutor executor;
    SafetyEngine safety;

    const std::string modelPath =
        "third_party/whisper.cpp/models/ggml-small.en.bin";

    VoiceEngine voiceEngine(modelPath);

    if (!voiceEngine.initialize())
    {
        std::cerr
            << "VoiceDesk: Voice subsystem initialization failed.\n";

        return 1;
    }

    std::cout
        << "VoiceDesk: Voice subsystem ready.\n\n";

    std::cout
        << "Speak your command now.\n";

    const std::string recognizedText =
        voiceEngine.listen();

    if (recognizedText.empty())
    {
        std::cerr
            << "VoiceDesk: No speech recognized.\n";

        return 1;
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

        return 1;
    }

    ParsedCommand command =
        parser.parse(recognizedText);

    if (command.type == CommandType::UNKNOWN)
    {
        std::cout
            << "VoiceDesk: Unknown command.\n";

        return 1;
    }

    if (command.type == CommandType::EXIT)
    {
        std::cout
            << "VoiceDesk: Goodbye.\n";

        return 0;
    }

    executor.execute(command);

    return 0;
}
