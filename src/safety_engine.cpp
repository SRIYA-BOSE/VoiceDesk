#include "safety_engine.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace
{
    std::string normalize(const std::string& input)
    {
        std::string result;

        result.reserve(input.size());

        for (unsigned char character : input)
        {
            if (std::isspace(character))
            {
                result += ' ';
            }
            else
            {
                result += static_cast<char>(
                    std::tolower(character)
                );
            }
        }

        std::string cleaned;

        cleaned.reserve(result.size());

        bool previousWasSpace = false;

        for (char character : result)
        {
            if (character == ' ')
            {
                if (!previousWasSpace)
                {
                    cleaned += character;
                }

                previousWasSpace = true;
            }
            else
            {
                cleaned += character;
                previousWasSpace = false;
            }
        }

        return cleaned;
    }

    bool containsAny(
        const std::string& command,
        const std::vector<std::string>& patterns
    )
    {
        for (const auto& pattern : patterns)
        {
            if (command.find(pattern) != std::string::npos)
            {
                return true;
            }
        }

        return false;
    }
}

bool SafetyEngine::isDangerous(
    const std::string& command
) const
{
    const std::string normalized =
        normalize(command);

    if (normalized.empty())
    {
        return false;
    }

    /*
     * Destructive filesystem operations.
     */
    const std::vector<std::string> filesystemPatterns =
    {
        "rm -rf /",
        "rm -fr /",
        "rm -r /",
        "rm --recursive --force /",
        "mkfs",
        "fdisk",
        "parted",
        "wipefs",
        "dd if=",
        "shred"
    };

    /*
     * System shutdown/reboot operations.
     */
    const std::vector<std::string> systemPatterns =
    {
        "shutdown",
        "reboot",
        "poweroff",
        "halt",
        "init 0",
        "init 6",
        "systemctl poweroff",
        "systemctl reboot",
        "systemctl halt"
    };

    /*
     * Dangerous privilege and permission operations.
     */
    const std::vector<std::string> privilegePatterns =
    {
        "chmod -r 777 /",
        "chmod -r 777 /*",
        "chmod -r 000 /",
        "chown -r",
        "chgrp -r"
    };

    /*
     * Fork bomb and shell recursion patterns.
     */
    const std::vector<std::string> shellPatterns =
    {
        ":(){",
        ":() {",
        "fork bomb",
        "while true; do",
        "while :; do"
    };

    /*
     * Network/system disruption patterns.
     */
    const std::vector<std::string> disruptionPatterns =
    {
        "iptables -f",
        "iptables --flush",
        "ufw disable",
        "systemctl stop networking",
        "systemctl stop networkmanager"
    };

    if (containsAny(normalized, filesystemPatterns))
    {
        return true;
    }

    if (containsAny(normalized, systemPatterns))
    {
        return true;
    }

    if (containsAny(normalized, privilegePatterns))
    {
        return true;
    }

    if (containsAny(normalized, shellPatterns))
    {
        return true;
    }

    if (containsAny(normalized, disruptionPatterns))
    {
        return true;
    }

    return false;
}

bool SafetyEngine::isSafe(
    const std::string& command
) const
{
    return !isDangerous(command);
}
