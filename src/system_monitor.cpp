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

void SystemMonitor::displayCPUUsage() const
{
    std::cout << "\n========== CPU Usage ==========\n\n";
    std::system("top -bn1 | grep -E '^%Cpu|Cpu\\(s\\)'");
    std::cout << "\n";
}

void SystemMonitor::displayMemoryUsage() const
{
    std::cout << "\n========== Memory Usage ==========\n\n";
    std::system("free -h");
    std::cout << "\n";
}

void SystemMonitor::displayDiskUsage() const
{
    std::cout << "\n========== Disk Usage ==========\n\n";
    std::system("df -h");
    std::cout << "\n";
}

void SystemMonitor::displayNetworkInformation() const
{
    std::cout << "\n========== Network Information ==========\n\n";
    std::system("ip -brief addr");
    std::cout << "\n";
}

void SystemMonitor::displayUptime() const
{
    std::cout << "\n========== System Uptime ==========\n\n";
    std::system("uptime");
    std::cout << "\n";
}

void SystemMonitor::displayLoadAverage() const
{
    std::cout << "\n========== Load Average ==========\n\n";
    std::system("cat /proc/loadavg");
    std::cout << "\n";
}

void SystemMonitor::displayKernelInformation() const
{
    std::cout << "\n========== Kernel Information ==========\n\n";
    std::system("uname -a");
    std::cout << "\n";
}
