#include "system_monitor.hpp"

#include <cstdlib>
#include <iostream>

void SystemMonitor::displaySystemInformation() const
{
    std::cout << "\n========== System Information ==========\n\n";

    std::cout << "Kernel:\n";
    std::system("uname -sr");

    std::cout << "\nHostname:\n";
    std::system("hostname");

    std::cout << "\nCPU:\n";
    std::system("lscpu | grep -E '^Model name|^CPU\\(s\\)'");

    std::cout << "\nMemory:\n";
    std::system("free -h");

    std::cout << "\nDisk:\n";
    std::system("df -h /");

    std::cout << "\n========================================\n\n";
}
