#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

class SystemMonitor
{
public:
    void displaySystemInformation() const;

    void displayCPUUsage() const;
    void displayMemoryUsage() const;
    void displayDiskUsage() const;
    void displayNetworkInformation() const;
    void displayUptime() const;
    void displayLoadAverage() const;
    void displayKernelInformation() const;
};

#endif
