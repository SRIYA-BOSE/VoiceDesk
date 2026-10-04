#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include <string>
#include <vector>

struct ApplicationInfo
{
    std::string name;
    std::string executable;
    std::string desktop_file;
};

class ApplicationManager
{
public:
    ApplicationManager();

    void refresh();

    bool launch(const std::string& application);

    bool close(const std::string& application);

    bool isAvailable(const std::string& application) const;

    std::vector<ApplicationInfo> getApplications() const;

private:
    std::vector<ApplicationInfo> applications_;

    std::string normalize(const std::string& value) const;

    std::string extractExecCommand(
        const std::string& desktop_file
    ) const;

    std::string extractName(
        const std::string& desktop_file
    ) const;

    bool launchCommand(
        const std::string& command
    ) const;
};

#endif
