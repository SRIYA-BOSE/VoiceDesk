#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>

class FileManager
{
public:
    void listFiles(const std::string& path) const;
    bool createFile(const std::string& filename) const;
    bool deleteFile(const std::string& filename) const;
};

#endif
