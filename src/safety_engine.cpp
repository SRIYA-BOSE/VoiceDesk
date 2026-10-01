#include "safety_engine.hpp"

#include <algorithm>
#include <cctype>
#include <vector>

namespace
{
    std::string normalize(const std::string& input)
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
}

bool SafetyEngine::isDangerous(const std::string& command) const
{
    const std::string normalized = normalize(command);

    const std::vector<std::string> blockedPatterns =
    {
        "rm -rf /",
        "rm -rf /*",
        "mkfs",
        "dd if=",
        ":(){ :|:& };:",
        "shutdown",
        "reboot",
        "poweroff",
        "init 0",
        "init 6",
        "chmod -r 777 /",
        "chown -r",
        "fork bomb"
    };

    for (const auto& pattern : blockedPatterns)
    {
        if (normalized.find(pattern) != std::string::npos)
        {
            return true;
        }
    }

    return false;
}

bool SafetyEngine::isSafe(const std::string& command) const
{
    return !isDangerous(command);
}
