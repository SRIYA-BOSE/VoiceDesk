#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <string>

enum class CommandType
{
    UNKNOWN,
    OPEN_APPLICATION,
    CLOSE_APPLICATION,
    SYSTEM_INFO,
    LIST_FILES,
    CREATE_FILE,
    DELETE_FILE,
    SHOW_PROCESSES,
    HELP,
    EXIT
};

struct ParsedCommand
{
    CommandType type;
    std::string argument;
    std::string original;
};

class CommandParser
{
public:
    ParsedCommand parse(const std::string& input) const;

private:
    std::string normalize(const std::string& input) const;
};

#endif
