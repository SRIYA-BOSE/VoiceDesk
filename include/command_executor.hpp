#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "command_parser.hpp"

class CommandExecutor
{
public:
    void execute(const ParsedCommand& command);

private:
    void showHelp();
};

#endif
