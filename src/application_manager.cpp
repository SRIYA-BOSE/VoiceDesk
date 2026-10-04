#include "application_manager.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

ApplicationManager::ApplicationManager()
{
    refresh();
}

std::string ApplicationManager::normalize(
    const std::string& value
) const
{
    std::string result;

    for (unsigned char c : value)
    {
        if (std::isalnum(c) || c == ' ')
        {
            result += static_cast<char>(
                std::tolower(c)
            );
        }
    }

    std::string cleaned;
    bool previousSpace = false;

    for (char c : result)
    {
        if (c == ' ')
        {
            if (!previousSpace)
            {
                cleaned += c;
            }

            previousSpace = true;
        }
        else
        {
            cleaned += c;
            previousSpace = false;
        }
    }

    while (
        !cleaned.empty() &&
        cleaned.front() == ' '
    )
    {
        cleaned.erase(cleaned.begin());
    }

    while (
        !cleaned.empty() &&
        cleaned.back() == ' '
    )
    {
        cleaned.pop_back();
    }

    return cleaned;
}

std::string ApplicationManager::extractName(
    const std::string& desktop_file
) const
{
    std::ifstream file(desktop_file);

    if (!file)
    {
        return {};
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("Name=", 0) == 0)
        {
            return line.substr(5);
        }
    }

    return {};
}

std::string ApplicationManager::extractExecCommand(
    const std::string& desktop_file
) const
{
    std::ifstream file(desktop_file);

    if (!file)
    {
        return {};
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("Exec=", 0) == 0)
        {
            std::string command =
                line.substr(5);

            const std::vector<std::string>
                placeholders =
            {
                "%U",
                "%u",
                "%F",
                "%f",
                "%i",
                "%c",
                "%k"
            };

            for (const auto& placeholder :
                 placeholders)
            {
                std::size_t position;

                while (
                    (position =
                        command.find(
                            placeholder
                        ))
                    != std::string::npos
                )
                {
                    command.erase(
                        position,
                        placeholder.length()
                    );
                }
            }

            while (
                !command.empty() &&
                std::isspace(
                    static_cast<unsigned char>(
                        command.back()
                    )
                )
            )
            {
                command.pop_back();
            }

            return command;
        }
    }

    return {};
}

void ApplicationManager::refresh()
{
    applications_.clear();

    std::vector<std::string>
        directories =
    {
        "/usr/share/applications",
        "/usr/local/share/applications"
    };

    const char* home =
        std::getenv("HOME");

    if (home != nullptr)
    {
        directories.push_back(
            std::string(home) +
            "/.local/share/applications"
        );
    }

    for (const auto& directory :
         directories)
    {
        if (!fs::exists(directory))
        {
            continue;
        }

        try
        {
            for (
                const auto& entry :
                fs::directory_iterator(
                    directory
                )
            )
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }

                if (
                    entry.path().extension()
                    != ".desktop"
                )
                {
                    continue;
                }

                const std::string file =
                    entry.path().string();

                const std::string name =
                    extractName(file);

                const std::string executable =
                    extractExecCommand(file);

                if (
                    name.empty() ||
                    executable.empty()
                )
                {
                    continue;
                }

                applications_.push_back(
                    {
                        name,
                        executable,
                        file
                    }
                );
            }
        }
        catch (
            const fs::filesystem_error&
        )
        {
            continue;
        }
    }

    std::sort(
        applications_.begin(),
        applications_.end(),
        [](const ApplicationInfo& a,
           const ApplicationInfo& b)
        {
            return a.name < b.name;
        }
    );

    std::cout
        << "[ApplicationManager] Discovered "
        << applications_.size()
        << " applications\n";
}

bool ApplicationManager::isAvailable(
    const std::string& application
) const
{
    const std::string target =
        normalize(application);

    if (target.empty())
    {
        return false;
    }

    for (const auto& app :
         applications_)
    {
        if (
            normalize(app.name) ==
            target
        )
        {
            return true;
        }
    }

    return false;
}

bool ApplicationManager::launchCommand(
    const std::string& command
) const
{
    if (command.empty())
    {
        return false;
    }

    const std::string fullCommand =
        command +
        " >/dev/null 2>&1 &";

    return std::system(
        fullCommand.c_str()
    ) == 0;
}

bool ApplicationManager::launch(
    const std::string& application
)
{
    const std::string target =
        normalize(application);

    if (target.empty())
    {
        return false;
    }

    /*
     * Explicit aliases.
     */
    if (target == "firefox")
    {
        return launchCommand("firefox");
    }

    if (
        target == "vscode" ||
        target == "vs code" ||
        target == "visual studio code"
    )
    {
        return launchCommand("code");
    }

    if (
        target == "terminal" ||
        target == "command terminal"
    )
    {
        return launchCommand("x-terminal-emulator");
    }

    if (
        target == "file manager" ||
        target == "files"
    )
    {
        return launchCommand(
            "xdg-open \"$HOME\""
        );
    }

    if (
        target == "calculator" ||
        target == "calc"
    )
    {
        return launchCommand(
            "gnome-calculator"
        );
    }

    if (
        target == "vlc" ||
        target == "vlc media player"
    )
    {
        return launchCommand("vlc");
    }

    /*
     * Exact application-name matching.
     */
    for (const auto& app :
         applications_)
    {
        const std::string name =
            normalize(app.name);

        if (name == target)
        {
            std::cout
                << "[ApplicationManager] Opening "
                << app.name
                << '\n';

            return launchCommand(
                app.executable
            );
        }
    }

    /*
     * Reject extremely short fuzzy targets.
     *
     * This prevents inputs such as "it", "a",
     * or "go" from accidentally matching an
     * unrelated application name.
     */
    if (target.length() < 3)
    {
        std::cout
            << "[ApplicationManager] "
               "Application target is too ambiguous: "
            << application
            << '\n';

        return false;
    }

    /*
     * Controlled partial matching.
     */
    for (const auto& app :
         applications_)
    {
        const std::string name =
            normalize(app.name);

        if (
            name.find(target) !=
            std::string::npos
        )
        {
            std::cout
                << "[ApplicationManager] "
                   "Matched application: "
                << app.name
                << '\n';

            return launchCommand(
                app.executable
            );
        }
    }

    return false;
}

bool ApplicationManager::close(
    const std::string& application
)
{
    const std::string target =
        normalize(application);

    if (target.empty())
    {
        return false;
    }

    std::string process;

    if (target == "firefox")
    {
        process = "firefox";
    }
    else if (
        target == "vscode" ||
        target == "vs code" ||
        target == "visual studio code"
    )
    {
        process = "code";
    }
    else if (
        target == "vlc" ||
        target == "vlc media player"
    )
    {
        process = "vlc";
    }
    else if (target == "terminal")
    {
        process = "x-terminal-emulator";
    }
    else
    {
        for (const auto& app :
             applications_)
        {
            const std::string name =
                normalize(app.name);

            if (name == target)
            {
                std::string command =
                    app.executable;

                const std::size_t space =
                    command.find(' ');

                if (
                    space !=
                    std::string::npos
                )
                {
                    command =
                        command.substr(
                            0,
                            space
                        );
                }

                process =
                    fs::path(command)
                        .filename()
                        .string();

                break;
            }
        }

        /*
         * Do not use fuzzy matching for closing
         * applications. Closing the wrong process
         * is more dangerous than failing to find one.
         */
    }

    if (process.empty())
    {
        return false;
    }

    std::cout
        << "[ApplicationManager] Closing "
        << application
        << '\n';

    const std::string command =
        "pkill -x \"" +
        process +
        "\"";

    return std::system(
        command.c_str()
    ) == 0;
}

std::vector<ApplicationInfo>
ApplicationManager::getApplications() const
{
    return applications_;
}
