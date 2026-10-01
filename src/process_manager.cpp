#include "process_manager.hpp"

#include <cstdlib>
#include <iostream>

void ProcessManager::listProcesses() const
{
    std::cout << "\n========== Running Processes ==========\n\n";

    const int result = std::system(
        "ps -eo pid,comm,%cpu,%mem --sort=-%cpu | head -20"
    );

    if (result != 0)
    {
        std::cerr << "Unable to retrieve process information.\n";
    }

    std::cout << "\n";
}
