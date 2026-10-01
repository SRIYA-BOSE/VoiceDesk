#ifndef SAFETY_ENGINE_HPP
#define SAFETY_ENGINE_HPP

#include <string>

class SafetyEngine
{
public:
    bool isSafe(const std::string& command) const;
    bool isDangerous(const std::string& command) const;
};

#endif
