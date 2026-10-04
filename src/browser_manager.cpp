#include "browser_manager.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

std::string BrowserManager::normalize(
    const std::string& value
) const
{
    std::string result;

    for (unsigned char c : value)
    {
        result += static_cast<char>(
            std::tolower(c)
        );
    }

    return result;
}

std::string BrowserManager::resolveWebsite(
    const std::string& target
) const
{
    const std::string name =
        normalize(target);

    if (
        name == "youtube" ||
        name == "youtube.com"
    )
    {
        return "https://www.youtube.com";
    }

    if (
        name == "whatsapp" ||
        name == "whatsapp web" ||
        name == "web whatsapp"
    )
    {
        return "https://web.whatsapp.com";
    }

    if (name == "gmail")
    {
        return "https://mail.google.com";
    }

    if (name == "google")
    {
        return "https://www.google.com";
    }

    if (name == "github")
    {
        return "https://github.com";
    }

    if (name == "linkedin")
    {
        return "https://www.linkedin.com";
    }

    if (name == "reddit")
    {
        return "https://www.reddit.com";
    }

    if (name == "chatgpt")
    {
        return "https://chatgpt.com";
    }

    if (
        name == "stackoverflow" ||
        name == "stack overflow"
    )
    {
        return "https://stackoverflow.com";
    }

    return {};
}

bool BrowserManager::openURL(
    const std::string& url
) const
{
    if (url.empty())
    {
        return false;
    }

    std::cout
        << "[BrowserManager] Opening: "
        << url
        << '\n';

    const std::string command =
        "xdg-open \"" +
        url +
        "\" >/dev/null 2>&1 &";

    return std::system(command.c_str()) == 0;
}

bool BrowserManager::open(
    const std::string& target
)
{
    if (
        target.rfind("http://", 0) == 0 ||
        target.rfind("https://", 0) == 0
    )
    {
        return openURL(target);
    }

    const std::string website =
        resolveWebsite(target);

    if (!website.empty())
    {
        return openURL(website);
    }

    if (
        target.find('.') != std::string::npos &&
        target.find(' ') == std::string::npos
    )
    {
        return openURL(
            "https://" + target
        );
    }

    return false;
}

std::string BrowserManager::urlEncode(
    const std::string& value
) const
{
    std::ostringstream encoded;

    for (unsigned char c : value)
    {
        if (
            std::isalnum(c) ||
            c == '-' ||
            c == '_' ||
            c == '.' ||
            c == '~'
        )
        {
            encoded << c;
        }
        else if (c == ' ')
        {
            encoded << '+';
        }
        else
        {
            encoded
                << '%'
                << std::uppercase
                << std::hex
                << static_cast<int>(c)
                << std::nouppercase
                << std::dec;
        }
    }

    return encoded.str();
}

bool BrowserManager::searchGoogle(
    const std::string& query
) const
{
    if (query.empty())
    {
        return false;
    }

    const std::string encoded =
        urlEncode(query);

    const std::string url =
        "https://www.google.com/search?q=" +
        encoded;

    return openURL(url);
}
