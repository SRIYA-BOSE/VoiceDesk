#ifndef BROWSER_MANAGER_HPP
#define BROWSER_MANAGER_HPP

#include "firefox_bidi_client.hpp"

#include <string>

class BrowserManager
{
public:
    BrowserManager() = default;

    bool open(const std::string& target);

    bool searchGoogle(const std::string& query) const;

    bool searchYouTube(const std::string& query) const;

private:
    std::string normalize(
        const std::string& value
    ) const;

    std::string resolveWebsite(
        const std::string& target
    ) const;

    bool openURL(
        const std::string& url
    ) const;

    std::string urlEncode(
        const std::string& value
    ) const;

    FirefoxBidiClient firefox_bidi_client_;
};

#endif
