#include "command_parser.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

std::string CommandParser::normalize(const std::string& input) const
{
    std::string result = input;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(std::tolower(c));
        });

    return result;
}

ParsedCommand CommandParser::parse(const std::string& input) const
{
    ParsedCommand command;

    command.type = CommandType::UNKNOWN;
    command.argument = "";
    command.original = input;

    const std::string text = normalize(input);

    if (text == "exit" ||
        text == "quit" ||
        text == "close assistant")
    {
        command.type = CommandType::EXIT;
    }
    else if (text == "help" ||
             text == "show help")
    {
        command.type = CommandType::HELP;
    }
    else if (text == "system information" ||
             text == "system info" ||
             text == "show system information")
    {
        command.type = CommandType::SYSTEM_INFO;
    }
    else if (text == "show processes" ||
             text == "list processes")
    {
        command.type = CommandType::SHOW_PROCESSES;
    }
    else if (text == "list files" ||
             text == "show files")
    {
        command.type = CommandType::LIST_FILES;
    }
    else if (text.rfind("open ", 0) == 0)
    {
        command.type = CommandType::OPEN_APPLICATION;
        command.argument = input.substr(5);
    }
    else if (text.rfind("close ", 0) == 0)
    {
        command.type = CommandType::CLOSE_APPLICATION;
        command.argument = input.substr(6);
    }
    else if (text.rfind("create file ", 0) == 0)
    {
        command.type = CommandType::CREATE_FILE;
        command.argument = input.substr(12);
    }
    else if (text.rfind("delete file ", 0) == 0)
    {
        command.type = CommandType::DELETE_FILE;
        command.argument = input.substr(12);
    }

    return command;
}
