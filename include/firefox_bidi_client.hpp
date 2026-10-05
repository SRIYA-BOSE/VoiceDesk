#ifndef FIREFOX_BIDI_CLIENT_HPP
#define FIREFOX_BIDI_CLIENT_HPP

#include <string>

class FirefoxBidiClient
{
public:
    FirefoxBidiClient() = default;

    bool isAvailable() const;

    bool navigate(
        const std::string& url
    ) const;

    bool evaluateScript(
        const std::string& expression,
        std::string& result
    ) const;

private:
    bool connect() const;

    bool performWebSocketHandshake(
        int socketFd
    ) const;

    bool createSession(
        int socketFd
    ) const;

    bool endSession(
        int socketFd
    ) const;

    bool getBrowsingContext(
        int socketFd,
        std::string& contextId
    ) const;

    bool navigateOnSession(
        int socketFd,
        const std::string& contextId,
        const std::string& url
    ) const;

    bool evaluateScriptOnSession(
        int socketFd,
        const std::string& expression,
        std::string& result
    ) const;

    std::string escapeJsonString(
        const std::string& value
    ) const;

    std::string generateWebSocketKey() const;

    std::string base64Encode(
        const unsigned char* data,
        std::size_t length
    ) const;

    bool sendWebSocketText(
        int socketFd,
        const std::string& payload
    ) const;

    bool receiveWebSocketText(
        int socketFd,
        std::string& payload
    ) const;
};

#endif
