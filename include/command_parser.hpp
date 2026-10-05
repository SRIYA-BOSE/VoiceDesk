#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <string>

enum class CommandType
{
    UNKNOWN,

    OPEN_APPLICATION,
    OPEN_URL,
    SEARCH_WEB,

    CLOSE_APPLICATION,

    SYSTEM_INFO,
    LIST_FILES,
    CREATE_FILE,
    DELETE_FILE,
    CREATE_FOLDER,
    DELETE_FOLDER,

    SHOW_PROCESSES,
    DISK_INFO,
    NETWORK_INFO,

    CPU_USAGE,
    MEMORY_USAGE,
    UPTIME,
    LOAD_AVERAGE,
    KERNEL_INFO,

    HELP,
    EXIT
};

struct ParsedCommand
{
    CommandType type;
    std::string argument;
};

class CommandParser
{
public:
    ParsedCommand parse(
        const std::string& input
    ) const;

private:
    std::string normalize(
        const std::string& input
    ) const;

    bool startsWith(
        const std::string& text,
        const std::string& prefix
    ) const;

    std::string removePrefix(
        const std::string& text,
        const std::string& prefix
    ) const;
};

#endif
