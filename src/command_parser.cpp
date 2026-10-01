#include "command_parser.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace
{
    std::string trim(const std::string& input)
    {
        const auto first =
            input.find_first_not_of(" \t\r\n");

        if (first == std::string::npos)
        {
            return "";
        }

        const auto last =
            input.find_last_not_of(" \t\r\n");

        return input.substr(first, last - first + 1);
    }

    std::string removeTrailingPunctuation(
        const std::string& input)
    {
        std::string result = trim(input);

        while (!result.empty())
        {
            const char last = result.back();

            if (last == '.' ||
                last == ',' ||
                last == '!' ||
                last == '?' ||
                last == ';' ||
                last == ':')
            {
                result.pop_back();
                result = trim(result);
            }
            else
            {
                break;
            }
        }

        return result;
    }

    bool startsWith(
        const std::string& text,
        const std::string& prefix)
    {
        return text.rfind(prefix, 0) == 0;
    }

    std::string removePrefix(
        const std::string& text,
        const std::string& prefix)
    {
        return trim(text.substr(prefix.length()));
    }

    std::string extractApplication(
        const std::string& text)
    {
        const std::vector<std::string> prefixes =
        {
            "please open ",
            "please launch ",
            "please start ",
            "can you open ",
            "can you launch ",
            "can you start ",
            "open ",
            "launch ",
            "start "
        };

        for (const auto& prefix : prefixes)
        {
            if (startsWith(text, prefix))
            {
                return removeTrailingPunctuation(
                    removePrefix(text, prefix));
            }
        }

        return "";
    }
}

std::string CommandParser::normalize(
    const std::string& input) const
{
    std::string result =
        removeTrailingPunctuation(input);

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

ParsedCommand CommandParser::parse(
    const std::string& input) const
{
    ParsedCommand command;

    command.type = CommandType::UNKNOWN;
    command.argument = "";
    command.original = input;

    const std::string text =
        normalize(input);

    /*
     * EXIT COMMANDS
     */
    if (text == "exit" ||
        text == "quit" ||
        text == "close assistant" ||
        text == "exit assistant")
    {
        command.type = CommandType::EXIT;
        return command;
    }

    /*
     * HELP COMMANDS
     */
    if (text == "help" ||
        text == "show help" ||
        text == "what can you do")
    {
        command.type = CommandType::HELP;
        return command;
    }

    /*
     * SYSTEM INFORMATION
     */
    if (text == "system information" ||
        text == "system info" ||
        text == "show system information" ||
        text == "show system info" ||
        text == "please show system information")
    {
        command.type = CommandType::SYSTEM_INFO;
        return command;
    }

    /*
     * PROCESS COMMANDS
     */
    if (text == "show processes" ||
        text == "list processes" ||
        text == "show running processes" ||
        text == "list running processes")
    {
        command.type = CommandType::SHOW_PROCESSES;
        return command;
    }

    /*
     * FILE LIST COMMANDS
     */
    if (text == "list files" ||
        text == "show files" ||
        text == "show my files")
    {
        command.type = CommandType::LIST_FILES;
        return command;
    }

    /*
     * APPLICATION OPEN COMMANDS
     */
    const std::string application =
        extractApplication(text);

    if (!application.empty())
    {
        command.type =
            CommandType::OPEN_APPLICATION;

        command.argument = application;

        return command;
    }

    /*
     * APPLICATION CLOSE COMMANDS
     */
    if (startsWith(text, "close "))
    {
        command.type =
            CommandType::CLOSE_APPLICATION;

        command.argument =
            removeTrailingPunctuation(
                removePrefix(text, "close "));

        return command;
    }

    if (startsWith(text, "please close "))
    {
        command.type =
            CommandType::CLOSE_APPLICATION;

        command.argument =
            removeTrailingPunctuation(
                removePrefix(text, "please close "));

        return command;
    }

    /*
     * CREATE FILE
     */
    if (startsWith(text, "create file "))
    {
        command.type =
            CommandType::CREATE_FILE;

        command.argument =
            removeTrailingPunctuation(
                removePrefix(text, "create file "));

        return command;
    }

    /*
     * DELETE FILE
     */
    if (startsWith(text, "delete file "))
    {
        command.type =
            CommandType::DELETE_FILE;

        command.argument =
            removeTrailingPunctuation(
                removePrefix(text, "delete file "));

        return command;
    }

    return command;
}
