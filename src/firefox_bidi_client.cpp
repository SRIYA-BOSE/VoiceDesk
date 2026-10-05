#include "firefox_bidi_client.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <openssl/rand.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{

const char base64Table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

bool sendAll(
    int socketFd,
    const unsigned char* data,
    std::size_t length
)
{
    std::size_t totalSent = 0;

    while (totalSent < length)
    {
        const ssize_t sent =
            send(
                socketFd,
                data + totalSent,
                length - totalSent,
                0
            );

        if (sent <= 0)
        {
            return false;
        }

        totalSent +=
            static_cast<std::size_t>(sent);
    }

    return true;
}

bool receiveAll(
    int socketFd,
    unsigned char* data,
    std::size_t length
)
{
    std::size_t totalReceived = 0;

    while (totalReceived < length)
    {
        const ssize_t received =
            recv(
                socketFd,
                data + totalReceived,
                length - totalReceived,
                0
            );

        if (received <= 0)
        {
            return false;
        }

        totalReceived +=
            static_cast<std::size_t>(received);
    }

    return true;
}

} // namespace

std::string FirefoxBidiClient::base64Encode(
    const unsigned char* data,
    std::size_t length
) const
{
    std::string result;

    result.reserve(
        ((length + 2) / 3) * 4
    );

    for (std::size_t i = 0; i < length; i += 3)
    {
        const unsigned int a = data[i];

        const unsigned int b =
            (i + 1 < length)
                ? data[i + 1]
                : 0;

        const unsigned int c =
            (i + 2 < length)
                ? data[i + 2]
                : 0;

        result +=
            base64Table[(a >> 2) & 0x3F];

        result +=
            base64Table[
                ((a & 0x03) << 4) |
                ((b >> 4) & 0x0F)
            ];

        result +=
            (i + 1 < length)
                ? base64Table[
                    ((b & 0x0F) << 2) |
                    ((c >> 6) & 0x03)
                ]
                : '=';

        result +=
            (i + 2 < length)
                ? base64Table[c & 0x3F]
                : '=';
    }

    return result;
}

std::string FirefoxBidiClient::escapeJsonString(
    const std::string& value
) const
{
    std::string escaped;
    escaped.reserve(value.size() + 16);

    for (unsigned char c : value)
    {
        switch (c)
        {
            case 34:
                escaped += "\\\"";
                break;

            case 92:
                escaped += "\\\\";
                break;

            case 10:
                escaped += "\\n";
                break;

            case 13:
                escaped += "\\r";
                break;

            case 9:
                escaped += "\\t";
                break;

            case 8:
                escaped += "\\b";
                break;

            case 12:
                escaped += "\\f";
                break;

            default:
                if (c < 0x20)
                {
                    char buffer[7];
                    std::snprintf(
                        buffer,
                        sizeof(buffer),
                        "\\u%04x",
                        static_cast<unsigned int>(c)
                    );
                    escaped += buffer;
                }
                else
                {
                    escaped += static_cast<char>(c);
                }
                break;
        }
    }

    return escaped;
}

std::string FirefoxBidiClient::generateWebSocketKey() const
{
    unsigned char randomBytes[16]{};

    if (
        RAND_bytes(
            randomBytes,
            sizeof(randomBytes)
        ) != 1
    )
    {
        return {};
    }

    return base64Encode(
        randomBytes,
        sizeof(randomBytes)
    );
}

bool FirefoxBidiClient::performWebSocketHandshake(
    int socketFd
) const
{
    const std::string key =
        generateWebSocketKey();

    if (key.empty())
    {
        return false;
    }

    const std::string request =
        "GET /session HTTP/1.1\r\n"
        "Host: 127.0.0.1:9222\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "Sec-WebSocket-Key: " +
        key +
        "\r\n"
        "\r\n";

    if (
        !sendAll(
            socketFd,
            reinterpret_cast<const unsigned char*>(
                request.data()
            ),
            request.size()
        )
    )
    {
        return false;
    }

    std::string response;

    char buffer[4096];

    while (
        response.find("\r\n\r\n") ==
        std::string::npos
    )
    {
        const ssize_t received =
            recv(
                socketFd,
                buffer,
                sizeof(buffer),
                0
            );

        if (received <= 0)
        {
            return false;
        }

        response.append(
            buffer,
            static_cast<std::size_t>(received)
        );

        if (response.size() > 65536)
        {
            return false;
        }
    }

    const std::size_t statusEnd =
        response.find("\r\n");

    if (statusEnd == std::string::npos)
    {
        return false;
    }

    const std::string statusLine =
        response.substr(
            0,
            statusEnd
        );

    return
        statusLine.find("101") !=
        std::string::npos;
}

bool FirefoxBidiClient::sendWebSocketText(
    int socketFd,
    const std::string& payload
) const
{
    std::string frame;

    frame.push_back(
        static_cast<char>(0x81)
    );

    const std::size_t length =
        payload.size();

    if (length <= 125)
    {
        frame.push_back(
            static_cast<char>(
                0x80 |
                static_cast<unsigned char>(
                    length
                )
            )
        );
    }
    else if (length <= 65535)
    {
        frame.push_back(
            static_cast<char>(0x80 | 126)
        );

        frame.push_back(
            static_cast<char>(
                (length >> 8) & 0xFF
            )
        );

        frame.push_back(
            static_cast<char>(
                length & 0xFF
            )
        );
    }
    else
    {
        return false;
    }

    unsigned char mask[4]{};

    if (
        RAND_bytes(
            mask,
            sizeof(mask)
        ) != 1
    )
    {
        return false;
    }

    for (const unsigned char value : mask)
    {
        frame.push_back(
            static_cast<char>(value)
        );
    }

    for (std::size_t i = 0; i < length; ++i)
    {
        frame.push_back(
            static_cast<char>(
                static_cast<unsigned char>(
                    payload[i]
                ) ^
                mask[i % 4]
            )
        );
    }

    return sendAll(
        socketFd,
        reinterpret_cast<const unsigned char*>(
            frame.data()
        ),
        frame.size()
    );
}

bool FirefoxBidiClient::receiveWebSocketText(
    int socketFd,
    std::string& payload
) const
{
    unsigned char header[2]{};

    if (
        !receiveAll(
            socketFd,
            header,
            sizeof(header)
        )
    )
    {
        return false;
    }

    const unsigned char opcode =
        header[0] & 0x0F;

    const bool masked =
        (header[1] & 0x80) != 0;

    std::uint64_t length =
        header[1] & 0x7F;

    if (opcode == 0x8)
    {
        return false;
    }

    if (length == 126)
    {
        unsigned char extended[2]{};

        if (
            !receiveAll(
                socketFd,
                extended,
                sizeof(extended)
            )
        )
        {
            return false;
        }

        length =
            (static_cast<std::uint64_t>(
                extended[0]
            ) << 8) |
            extended[1];
    }
    else if (length == 127)
    {
        unsigned char extended[8]{};

        if (
            !receiveAll(
                socketFd,
                extended,
                sizeof(extended)
            )
        )
        {
            return false;
        }

        length = 0;

        for (unsigned char value : extended)
        {
            length =
                (length << 8) |
                value;
        }
    }

    unsigned char mask[4]{};

    if (masked)
    {
        if (
            !receiveAll(
                socketFd,
                mask,
                sizeof(mask)
            )
        )
        {
            return false;
        }
    }

    if (length > 1024 * 1024)
    {
        return false;
    }

    payload.resize(
        static_cast<std::size_t>(length)
    );

    if (length > 0)
    {
        if (
            !receiveAll(
                socketFd,
                reinterpret_cast<unsigned char*>(
                    payload.data()
                ),
                static_cast<std::size_t>(length)
            )
        )
        {
            return false;
        }
    }

    if (masked)
    {
        for (std::size_t i = 0; i < length; ++i)
        {
            payload[i] =
                static_cast<char>(
                    static_cast<unsigned char>(
                        payload[i]
                    ) ^
                    mask[i % 4]
                );
        }
    }

    return opcode == 0x1;
}

bool FirefoxBidiClient::createSession(
    int socketFd
) const
{
    const std::string request =
        R"({"id":1,"method":"session.new","params":{"capabilities":{"alwaysMatch":{}}}})";

    if (!sendWebSocketText(socketFd, request))
    {
        std::cerr << "[BiDi] session.new send failed" << std::endl;
        return false;
    }

    std::string response;

    if (!receiveWebSocketText(socketFd, response))
    {
        return false;
    }


    std::cerr << "[BiDi] session.new response:" << std::endl;
    std::cerr << response << std::endl;

    return
        response.find("\"id\":1") !=
        std::string::npos &&
        response.find("\"result\"") !=
        std::string::npos;
}

bool FirefoxBidiClient::endSession(
    int socketFd
) const
{
    const std::string request =
        R"({"id":4,"method":"session.end","params":{}})";

    if (!sendWebSocketText(socketFd, request))
    {
        return false;
    }

    std::string response;

    if (!receiveWebSocketText(socketFd, response))
    {
        return false;
    }

    return
        response.find("\"id\":4") !=
        std::string::npos &&
        response.find("\"result\"") !=
        std::string::npos;
}

bool FirefoxBidiClient::getBrowsingContext(
    int socketFd,
    std::string& contextId
) const
{
    const std::string request =
        R"({"id":2,"method":"browsingContext.getTree","params":{}})";

    if (!sendWebSocketText(socketFd, request))
    {
        return false;
    }

    std::string response;

    if (!receiveWebSocketText(socketFd, response))
    {
        return false;
    }

    std::cerr << "[BiDi] getTree response:" << std::endl;
    std::cerr << response << std::endl;

    const std::string marker = "\"context\":\"";
    const std::size_t start = response.find(marker);

    if (start == std::string::npos)
    {
        return false;
    }

    const std::size_t contextStart =
        start + marker.size();

    const std::size_t contextEnd =
        response.find("\"", contextStart);

    if (contextEnd == std::string::npos ||
        contextEnd == contextStart)
    {
        return false;
    }

    contextId = response.substr(
        contextStart,
        contextEnd - contextStart
    );

    return !contextId.empty();
}

bool FirefoxBidiClient::navigateOnSession(
    int socketFd,
    const std::string& contextId,
    const std::string& url
) const
{
    const std::string request =
        std::string(
            R"({"id":5,"method":"browsingContext.navigate","params":{"context":")"
        ) +
        contextId +
        R"(","url":")" +
        url +
        R"(","wait":"complete"}})";

    if (!sendWebSocketText(socketFd, request))
    {
        return false;
    }

    std::string response;

    if (!receiveWebSocketText(socketFd, response))
    {
        return false;
    }

    std::cerr << "[BiDi] navigate response:" << std::endl;
    std::cerr << response << std::endl;

    return
        response.find("\"id\":5") !=
        std::string::npos &&
        response.find("\"result\"") !=
        std::string::npos;
}

bool FirefoxBidiClient::evaluateScriptOnSession(
    int socketFd,
    const std::string& expression,
    std::string& result
) const
{
    std::string contextId;

    if (!getBrowsingContext(socketFd, contextId))
    {
        return false;
    }
    const std::string escapedExpression =
        escapeJsonString(expression);

    const std::string request =
        std::string(
            R"({"id":3,"method":"script.evaluate","params":{"expression":")"
        ) +
        escapedExpression +
        R"(","target":{"context":")" + contextId + R"("},"awaitPromise":false,"resultOwnership":"none"}})";

    if (
        !sendWebSocketText(
            socketFd,
            request
        )
    )
    {
        return false;
    }

    return receiveWebSocketText(
        socketFd,
        result
    );
}

bool FirefoxBidiClient::connect() const
{
    const int socketFd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if (socketFd < 0)
    {
        return false;
    }

    sockaddr_in address{};

    address.sin_family =
        AF_INET;

    address.sin_port =
        htons(9222);

    if (
        inet_pton(
            AF_INET,
            "127.0.0.1",
            &address.sin_addr
        ) != 1
    )
    {
        close(socketFd);
        return false;
    }

    if (
        ::connect(
            socketFd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) != 0
    )
    {
        close(socketFd);
        return false;
    }

    if (
        !performWebSocketHandshake(
            socketFd
        )
    )
    {
        close(socketFd);
        return false;
    }

    const bool sessionCreated =
        createSession(socketFd);

    if (!sessionCreated)
    {
        close(socketFd);
        return false;
    }

    const bool sessionEnded =
        endSession(socketFd);

    close(socketFd);

    return sessionEnded;
}

bool FirefoxBidiClient::navigate(
    const std::string& url
) const
{
    if (url.empty())
    {
        return false;
    }

    const int socketFd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if (socketFd < 0)
    {
        return false;
    }

    sockaddr_in address{};

    address.sin_family =
        AF_INET;

    address.sin_port =
        htons(9222);

    if (
        inet_pton(
            AF_INET,
            "127.0.0.1",
            &address.sin_addr
        ) != 1
    )
    {
        close(socketFd);
        return false;
    }

    if (
        ::connect(
            socketFd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) != 0
    )
    {
        close(socketFd);
        return false;
    }

    if (
        !performWebSocketHandshake(
            socketFd
        )
    )
    {
        close(socketFd);
        return false;
    }

    if (
        !createSession(socketFd)
    )
    {
        close(socketFd);
        return false;
    }

    std::string contextId;

    const bool contextFound =
        getBrowsingContext(
            socketFd,
            contextId
        );

    bool navigationSucceeded = false;

    if (contextFound)
    {
        navigationSucceeded =
            navigateOnSession(
                socketFd,
                contextId,
                url
            );
    }

    const bool sessionEnded =
        endSession(socketFd);

    close(socketFd);

    return
        navigationSucceeded &&
        sessionEnded;
}

bool FirefoxBidiClient::evaluateScript(
    const std::string& expression,
    std::string& result
) const
{
    const int socketFd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if (socketFd < 0)
    {
        return false;
    }

    sockaddr_in address{};

    address.sin_family =
        AF_INET;

    address.sin_port =
        htons(9222);

    if (
        inet_pton(
            AF_INET,
            "127.0.0.1",
            &address.sin_addr
        ) != 1
    )
    {
        close(socketFd);
        return false;
    }

    if (
        ::connect(
            socketFd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) != 0
    )
    {
        close(socketFd);
        return false;
    }

    if (
        !performWebSocketHandshake(
            socketFd
        )
    )
    {
        close(socketFd);
        return false;
    }

    if (
        !createSession(socketFd)
    )
    {
        close(socketFd);
        return false;
    }

    const bool success =
        evaluateScriptOnSession(
            socketFd,
            expression,
            result
        );

    const bool sessionEnded =
        endSession(socketFd);

    close(socketFd);

    return success && sessionEnded;

}

bool FirefoxBidiClient::isAvailable() const
{
    return connect();
}
