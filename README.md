# VoiceDesk

## Voice-Controlled Linux Desktop Assistant

VoiceDesk is a **C++17 voice-controlled Linux desktop assistant** designed to demonstrate the integration of **speech recognition, Linux system programming, command safety, process management, file management, and a custom Linux character device driver**.

The system allows users to interact with selected Linux desktop operations using natural-language commands through either keyboard or voice input.

---

## 🚀 Features

- 🎙️ Local voice recognition using **whisper.cpp**
- 🧠 Natural-language command parsing
- 🛡️ Dangerous-command safety validation
- 🖥️ Controlled Linux application management
- 📂 File management
- ⚙️ Process management
- 📊 System information monitoring
- 🔌 Custom Linux character device driver
- 🔄 User-space ↔ kernel-space communication
- 🔐 Application allowlisting
- 🧩 Modular C++17 architecture
- 🐧 Linux system programming
- 🔧 Linux kernel module development
- 📚 Complete technical documentation

---

## 📌 Project Overview

VoiceDesk provides a voice-driven interface for interacting with selected Linux desktop functions.

A user can enter commands such as:

```text
Open Firefox
Show system information
Show processes
List files
Create file test.txt
Delete file test.txt
Close Firefox
Help
Exit
```

The system processes each command through a controlled pipeline:

```text
┌─────────────────────────────────────────────┐
│              User Interaction               │
│                                             │
│       Keyboard Input / Voice Input          │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Voice Recognition               │
│                                             │
│                 whisper.cpp                 │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│               Safety Engine                 │
│                                             │
│       Dangerous Command Detection           │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              Command Parser                 │
│                                             │
│    Natural Language → Structured Command    │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Command Executor                │
│                                             │
│ Application / File / Process / System Ops   │
└───────────────┬─────────────────┬───────────┘
                │                 │
                │                 ▼
                │        ┌────────────────────┐
                │        │ Device Driver      │
                │        │ Client             │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │        ┌────────────────────┐
                │        │ /dev/voicedesk     │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │        ┌────────────────────┐
                │        │ Linux Character    │
                │        │ Device Driver      │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │             Kernel Space
                │
                ▼
        Linux User-Space Operations
```

---

# 🏗️ System Architecture

VoiceDesk follows a layered architecture that separates voice recognition, safety validation, command interpretation, execution, and kernel communication.

```text
                    ┌─────────────────────┐
                    │        USER         │
                    │                     │
                    │ Keyboard / Voice    │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    VoiceEngine      │
                    │     whisper.cpp     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    SafetyEngine     │
                    │ Command Validation  │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   CommandParser     │
                    │ Intent Recognition  │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │  CommandExecutor    │
                    └──────────┬──────────┘
                               │
                  ┌────────────┴────────────┐
                  │                         │
                  ▼                         ▼
        ┌──────────────────┐      ┌──────────────────┐
        │ Linux User Space │      │ Device Driver    │
        │ Operations       │      │ Client           │
        └──────────────────┘      └────────┬─────────┘
                                           │
                                           ▼
                                  ┌──────────────────┐
                                  │ /dev/voicedesk   │
                                  └────────┬─────────┘
                                           │
                                           ▼
                                  ┌──────────────────┐
                                  │ Linux Character  │
                                  │ Device Driver    │
                                  └────────┬─────────┘
                                           │
                                           ▼
                                      Kernel Space
```

---

# 🔄 Command Processing Pipeline

Every command passes through a controlled processing sequence:

```text
User Input
    │
    ▼
Safety Validation
    │
    ├────────────── Unsafe ──────────────► BLOCK
    │
    ▼
Command Parser
    │
    ▼
Structured Command
    │
    ▼
Command Executor
    │
    ├──────────────► Linux User-Space Operation
    │
    ▼
Device Driver Client
    │
    ▼
/dev/voicedesk
    │
    ▼
Linux Character Driver
    │
    ▼
Kernel Space
```

This separation allows the system to keep **command validation, command interpretation, execution, and kernel communication** as distinct components.

---

# 🎙️ Voice Recognition

VoiceDesk uses **whisper.cpp** for local speech-to-text processing.

The voice pipeline is:

```text
Microphone
    │
    ▼
Audio Capture
    │
    ▼
whisper.cpp
    │
    ▼
Recognized Text
    │
    ▼
Safety Engine
    │
    ▼
Command Parser
    │
    ▼
Command Executor
```

The project uses an English Whisper model.

The downloaded model is intentionally excluded from the Git repository because model files are large.

---

# 🧠 Natural-Language Command Parsing

VoiceDesk does not require every command to follow exactly the same wording.

For example, the following inputs can represent the same application-opening intent:

```text
open firefox
Open Firefox
please open Firefox
can you open Firefox
launch Firefox
start Firefox
```

The command parser converts natural-language input into structured command types.

Supported command categories include:

```text
UNKNOWN
OPEN_APPLICATION
CLOSE_APPLICATION
SYSTEM_INFO
LIST_FILES
CREATE_FILE
DELETE_FILE
SHOW_PROCESSES
HELP
EXIT
```

---

# 🛡️ Safety Engine

VoiceDesk performs safety validation before executing a command.

The safety architecture is:

```text
Raw User Input
      │
      ▼
SafetyEngine
      │
      ├──────── Unsafe ────────► BLOCK
      │
      ▼
CommandParser
      │
      ▼
CommandExecutor
```

The safety layer detects known dangerous command patterns, including examples such as:

```text
rm -rf /
rm -rf /*
mkfs
dd if=
shutdown
reboot
poweroff
```

Example:

```text
User:
rm -rf /

VoiceDesk:
[Safety] Command blocked for security reasons.
```

The safety engine operates in user space before the command reaches the execution stage.

The Linux character driver provides communication between user space and kernel space; it is not the application's primary safety authority.

---

# 🖥️ Application Management

VoiceDesk uses a controlled application allowlist instead of unrestricted arbitrary application execution.

Current supported examples include:

```text
firefox
xterm
gedit
nautilus
```

Example:

```text
open firefox
```

and:

```text
close firefox
```

The executor validates the requested application before performing the operation.

---

# 📂 File Management

VoiceDesk supports controlled file operations.

### List files

```text
list files
```

### Create a file

```text
create file test.txt
```

### Delete a file

```text
delete file test.txt
```

The file-management component provides the application-level interface for these operations.

---

# ⚙️ Process Management

VoiceDesk can display running Linux processes.

Command:

```text
show processes
```

The process-management component interacts with Linux process information to present the current process state.

---

# 📊 System Monitoring

VoiceDesk provides system information through Linux system interfaces.

Command:

```text
show system information
```

The system-monitoring component can provide information about the Linux environment, kernel, CPU, memory, and other available system details.

---

# 🔌 Linux Character Device Driver

VoiceDesk includes a custom Linux character device driver located at:

```text
driver/voicedesk_driver.c
```

The driver provides the device interface:

```text
/dev/voicedesk
```

The driver demonstrates practical Linux kernel programming concepts including:

- Linux kernel modules
- Character devices
- Major and minor numbers
- `cdev`
- File operations
- Device classes
- `device_create()`
- `copy_to_user()`
- `copy_from_user()`
- Kernel mutex synchronization
- Kernel logging
- Module initialization
- Module cleanup

---

# 🔄 User-Space ↔ Kernel-Space Communication

VoiceDesk demonstrates communication between the C++ user-space application and the Linux kernel driver.

```text
┌──────────────────────────────┐
│       VoiceDesk C++          │
│          User Space          │
└──────────────┬───────────────┘
               │
               │ open()
               │ write()
               │ read()
               │ close()
               ▼
┌──────────────────────────────┐
│       /dev/voicedesk         │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│   Linux Character Driver     │
│         Kernel Space         │
└──────────────────────────────┘
```

The application contains a dedicated driver client:

```text
include/device_driver_client.hpp
src/device_driver_client.cpp
```

The client uses standard Linux file operations to communicate with the device.

Conceptually:

```cpp
DeviceDriverClient driver_("/dev/voicedesk");

driver_.sendCommand("open firefox");
```

---

# 🔐 Driver Synchronization

The character driver maintains an internal command buffer protected using a Linux kernel mutex.

Conceptually:

```text
Process A ──┐
Process B ──┼──► Mutex ──► Driver Buffer
Process C ──┘
```

The mutex protects shared driver state from concurrent access.

---

# 🧱 Software Components

## VoiceEngine

Handles voice input and speech recognition using whisper.cpp.

## SafetyEngine

Validates commands before they reach the execution layer.

## CommandParser

Converts natural-language commands into structured command types.

## CommandExecutor

Coordinates command execution and driver communication.

## FileManager

Handles supported file-management operations.

## ProcessManager

Provides process-related operations.

## SystemMonitor

Provides Linux system information.

## DeviceDriverClient

Provides the user-space interface to:

```text
/dev/voicedesk
```

## Linux Character Driver

Provides the kernel-space communication endpoint.

---

# 📁 Project Structure

```text
VoiceDesk/
│
├── Makefile
├── README.md
├── .gitignore
│
├── build/
│
├── docs/
│   ├── architecture.md
│   └── installation.md
│
├── driver/
│   ├── Makefile
│   ├── README.md
│   └── voicedesk_driver.c
│
├── include/
│   ├── command_executor.hpp
│   ├── command_parser.hpp
│   ├── device_driver_client.hpp
│   ├── file_manager.hpp
│   ├── process_manager.hpp
│   ├── safety_engine.hpp
│   ├── system_monitor.hpp
│   └── voice_engine.hpp
│
├── src/
│   ├── command_executor.cpp
│   ├── command_parser.cpp
│   ├── device_driver_client.cpp
│   ├── file_manager.cpp
│   ├── main.cpp
│   ├── process_manager.cpp
│   ├── safety_engine.cpp
│   ├── system_monitor.cpp
│   └── voice_engine.cpp
│
├── tests/
│
└── third_party/
    └── whisper.cpp/
```

---

# 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++17 | Main application |
| Linux | Target operating system |
| Linux Kernel | Character device driver |
| whisper.cpp | Local speech recognition |
| GCC / G++ | C++ compilation |
| GNU Make | Build system |
| CMake | whisper.cpp build |
| Git | Version control |
| GitHub | Source-code hosting |
| PulseAudio | Linux audio capture |
| WSL2 | Development environment |

---

# 📋 Requirements

VoiceDesk requires a Linux environment with:

- GCC / G++
- GNU Make
- CMake
- Git
- Linux development tools
- Compatible Linux kernel development/source environment for the driver
- whisper.cpp dependencies
- Audio support for voice functionality

Verify the main development tools:

```bash
uname -a
g++ --version
git --version
cmake --version
make --version
```

---

# 📥 Installation

## 1. Clone the Repository

```bash
git clone https://github.com/SRIYA-BOSE/VoiceDesk.git
cd VoiceDesk
```

Initialize the whisper.cpp submodule:

```bash
git submodule update --init --recursive
```

---

## 2. Build whisper.cpp

Enter the whisper.cpp directory:

```bash
cd third_party/whisper.cpp
```

Configure:

```bash
cmake -B build -DWHISPER_SDL2=ON
```

Build:

```bash
cmake --build build -j1 --config Release
```

Return to the project root:

```bash
cd ../..
```

---

## 3. Add the Whisper Model

Place the required English Whisper model at:

```text
third_party/whisper.cpp/models/ggml-base.en.bin
```

The model is excluded from Git through `.gitignore`.

---

## 4. Build VoiceDesk

From the project root:

```bash
make
```

The executable will be generated at:

```text
build/voicedesk
```

---

# 🔧 Linux Character Driver Setup

## 1. Build the Driver

Enter the driver directory:

```bash
cd driver
```

Build:

```bash
make
```

The resulting module is:

```text
voicedesk_driver.ko
```

Return to the project root:

```bash
cd ..
```

> The driver must be built against a compatible Linux kernel development/source environment.

---

## 2. Load the Driver

```bash
sudo insmod driver/voicedesk_driver.ko
```

Verify the module:

```bash
lsmod | grep voicedesk
```

Verify the device:

```bash
ls -l /dev/voicedesk
```

Check kernel messages:

```bash
dmesg | grep voicedesk
```

---

## 3. Device Permissions

A udev rule can be configured as:

```text
KERNEL=="voicedesk", MODE="0660", GROUP="users"
```

Reload udev rules:

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Verify:

```bash
ls -l /dev/voicedesk
```

---

# ▶️ Running VoiceDesk

Start the application:

```bash
./build/voicedesk
```

Example keyboard interaction:

```text
VoiceDesk> open firefox
```

For voice interaction:

```text
VoiceDesk> voice
```

Then speak a supported command.

---

# 🧪 Testing

## System Information

```text
show system information
```

## Running Processes

```text
show processes
```

## List Files

```text
list files
```

## Create File

```text
create file test.txt
```

Verify:

```bash
ls -l test.txt
```

## Delete File

```text
delete file test.txt
```

Verify:

```bash
ls -l test.txt
```

## Open Application

```text
open firefox
```

## Close Application

```text
close firefox
```

## Help

```text
help
```

## Exit

```text
exit
```

---

# 🛡️ Safety Test

A key project demonstration is the safety layer.

Enter:

```text
rm -rf /
```

Expected:

```text
[Safety] Command blocked for security reasons.
```

The dangerous command should not be executed.

This demonstrates the safety path:

```text
User Input
    ↓
SafetyEngine
    ↓
BLOCK
```

---

# 🔍 Driver Verification

Check whether the driver is loaded:

```bash
lsmod | grep voicedesk
```

Check the device:

```bash
ls -l /dev/voicedesk
```

Check kernel logs:

```bash
dmesg | grep voicedesk
```

Typical driver messages include:

```text
voicedesk: driver loaded
voicedesk: device opened
voicedesk: received command: open firefox
voicedesk: device closed
```

These messages demonstrate the communication path between the VoiceDesk application and the Linux character driver.

---
# VoiceDesk

## Voice-Controlled Linux Desktop Assistant

VoiceDesk is a **C++17 voice-controlled Linux desktop assistant** designed to demonstrate the integration of **speech recognition, Linux system programming, command safety, process management, file management, and a custom Linux character device driver**.

The system allows users to interact with selected Linux desktop operations using natural-language commands through either keyboard or voice input.

---

## 🚀 Features

- 🎙️ Local voice recognition using **whisper.cpp**
- 🧠 Natural-language command parsing
- 🛡️ Dangerous-command safety validation
- 🖥️ Controlled Linux application management
- 📂 File management
- ⚙️ Process management
- 📊 System information monitoring
- 🔌 Custom Linux character device driver
- 🔄 User-space ↔ kernel-space communication
- 🔐 Application allowlisting
- 🧩 Modular C++17 architecture
- 🐧 Linux system programming
- 🔧 Linux kernel module development
- 📚 Complete technical documentation

---

## 📌 Project Overview

VoiceDesk provides a voice-driven interface for interacting with selected Linux desktop functions.

A user can enter commands such as:

```text
Open Firefox
Show system information
Show processes
List files
Create file test.txt
Delete file test.txt
Close Firefox
Help
Exit
```

The system processes each command through a controlled pipeline:

```text
┌─────────────────────────────────────────────┐
│              User Interaction               │
│                                             │
│       Keyboard Input / Voice Input          │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Voice Recognition               │
│                                             │
│                 whisper.cpp                 │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│               Safety Engine                 │
│                                             │
│       Dangerous Command Detection           │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              Command Parser                 │
│                                             │
│    Natural Language → Structured Command    │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             Command Executor                │
│                                             │
│ Application / File / Process / System Ops   │
└───────────────┬─────────────────┬───────────┘
                │                 │
                │                 ▼
                │        ┌────────────────────┐
                │        │ Device Driver      │
                │        │ Client             │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │        ┌────────────────────┐
                │        │ /dev/voicedesk     │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │        ┌────────────────────┐
                │        │ Linux Character    │
                │        │ Device Driver      │
                │        └─────────┬──────────┘
                │                  │
                │                  ▼
                │             Kernel Space
                │
                ▼
        Linux User-Space Operations
```

---

# 🏗️ System Architecture

VoiceDesk follows a layered architecture that separates voice recognition, safety validation, command interpretation, execution, and kernel communication.

```text
                    ┌─────────────────────┐
                    │        USER         │
                    │                     │
                    │ Keyboard / Voice    │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    VoiceEngine      │
                    │     whisper.cpp     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    SafetyEngine     │
                    │ Command Validation  │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   CommandParser     │
                    │ Intent Recognition  │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │  CommandExecutor    │
                    └──────────┬──────────┘
                               │
                  ┌────────────┴────────────┐
                  │                         │
                  ▼                         ▼
        ┌──────────────────┐      ┌──────────────────┐
        │ Linux User Space │      │ Device Driver    │
        │ Operations       │      │ Client           │
        └──────────────────┘      └────────┬─────────┘
                                           │
                                           ▼
                                  ┌──────────────────┐
                                  │ /dev/voicedesk   │
                                  └────────┬─────────┘
                                           │
                                           ▼
                                  ┌──────────────────┐
                                  │ Linux Character  │
                                  │ Device Driver    │
                                  └────────┬─────────┘
                                           │
                                           ▼
                                      Kernel Space
```

---

# 🔄 Command Processing Pipeline

Every command passes through a controlled processing sequence:

```text
User Input
    │
    ▼
Safety Validation
    │
    ├────────────── Unsafe ──────────────► BLOCK
    │
    ▼
Command Parser
    │
    ▼
Structured Command
    │
    ▼
Command Executor
    │
    ├──────────────► Linux User-Space Operation
    │
    ▼
Device Driver Client
    │
    ▼
/dev/voicedesk
    │
    ▼
Linux Character Driver
    │
    ▼
Kernel Space
```

This separation allows the system to keep **command validation, command interpretation, execution, and kernel communication** as distinct components.

---

# 🎙️ Voice Recognition

VoiceDesk uses **whisper.cpp** for local speech-to-text processing.

The voice pipeline is:

```text
Microphone
    │
    ▼
Audio Capture
    │
    ▼
whisper.cpp
    │
    ▼
Recognized Text
    │
    ▼
Safety Engine
    │
    ▼
Command Parser
    │
    ▼
Command Executor
```

The project uses an English Whisper model.

The downloaded model is intentionally excluded from the Git repository because model files are large.

---

# 🧠 Natural-Language Command Parsing

VoiceDesk does not require every command to follow exactly the same wording.

For example, the following inputs can represent the same application-opening intent:

```text
open firefox
Open Firefox
please open Firefox
can you open Firefox
launch Firefox
start Firefox
```

The command parser converts natural-language input into structured command types.

Supported command categories include:

```text
UNKNOWN
OPEN_APPLICATION
CLOSE_APPLICATION
SYSTEM_INFO
LIST_FILES
CREATE_FILE
DELETE_FILE
SHOW_PROCESSES
HELP
EXIT
```

---

# 🛡️ Safety Engine

VoiceDesk performs safety validation before executing a command.

The safety architecture is:

```text
Raw User Input
      │
      ▼
SafetyEngine
      │
      ├──────── Unsafe ────────► BLOCK
      │
      ▼
CommandParser
      │
      ▼
CommandExecutor
```

The safety layer detects known dangerous command patterns, including examples such as:

```text
rm -rf /
rm -rf /*
mkfs
dd if=
shutdown
reboot
poweroff
```

Example:

```text
User:
rm -rf /

VoiceDesk:
[Safety] Command blocked for security reasons.
```

The safety engine operates in user space before the command reaches the execution stage.

The Linux character driver provides communication between user space and kernel space; it is not the application's primary safety authority.

---

# 🖥️ Application Management

VoiceDesk uses a controlled application allowlist instead of unrestricted arbitrary application execution.

Current supported examples include:

```text
firefox
xterm
gedit
nautilus
```

Example:

```text
open firefox
```

and:

```text
close firefox
```

The executor validates the requested application before performing the operation.

---

# 📂 File Management

VoiceDesk supports controlled file operations.

### List files

```text
list files
```

### Create a file

```text
create file test.txt
```

### Delete a file

```text
delete file test.txt
```

The file-management component provides the application-level interface for these operations.

---

# ⚙️ Process Management

VoiceDesk can display running Linux processes.

Command:

```text
show processes
```

The process-management component interacts with Linux process information to present the current process state.

---

# 📊 System Monitoring

VoiceDesk provides system information through Linux system interfaces.

Command:

```text
show system information
```

The system-monitoring component can provide information about the Linux environment, kernel, CPU, memory, and other available system details.

---

# 🔌 Linux Character Device Driver

VoiceDesk includes a custom Linux character device driver located at:

```text
driver/voicedesk_driver.c
```

The driver provides the device interface:

```text
/dev/voicedesk
```

The driver demonstrates practical Linux kernel programming concepts including:

- Linux kernel modules
- Character devices
- Major and minor numbers
- `cdev`
- File operations
- Device classes
- `device_create()`
- `copy_to_user()`
- `copy_from_user()`
- Kernel mutex synchronization
- Kernel logging
- Module initialization
- Module cleanup

---

# 🔄 User-Space ↔ Kernel-Space Communication

VoiceDesk demonstrates communication between the C++ user-space application and the Linux kernel driver.

```text
┌──────────────────────────────┐
│       VoiceDesk C++          │
│          User Space          │
└──────────────┬───────────────┘
               │
               │ open()
               │ write()
               │ read()
               │ close()
               ▼
┌──────────────────────────────┐
│       /dev/voicedesk         │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│   Linux Character Driver     │
│         Kernel Space         │
└──────────────────────────────┘
```

The application contains a dedicated driver client:

```text
include/device_driver_client.hpp
src/device_driver_client.cpp
```

The client uses standard Linux file operations to communicate with the device.

Conceptually:

```cpp
DeviceDriverClient driver_("/dev/voicedesk");

driver_.sendCommand("open firefox");
```

---

# 🔐 Driver Synchronization

The character driver maintains an internal command buffer protected using a Linux kernel mutex.

Conceptually:

```text
Process A ──┐
Process B ──┼──► Mutex ──► Driver Buffer
Process C ──┘
```

The mutex protects shared driver state from concurrent access.

---

# 🧱 Software Components

## VoiceEngine

Handles voice input and speech recognition using whisper.cpp.

## SafetyEngine

Validates commands before they reach the execution layer.

## CommandParser

Converts natural-language commands into structured command types.

## CommandExecutor

Coordinates command execution and driver communication.

## FileManager

Handles supported file-management operations.

## ProcessManager

Provides process-related operations.

## SystemMonitor

Provides Linux system information.

## DeviceDriverClient

Provides the user-space interface to:

```text
/dev/voicedesk
```

## Linux Character Driver

Provides the kernel-space communication endpoint.

---

# 📁 Project Structure

```text
VoiceDesk/
│
├── Makefile
├── README.md
├── .gitignore
│
├── build/
│
├── docs/
│   ├── architecture.md
│   └── installation.md
│
├── driver/
│   ├── Makefile
│   ├── README.md
│   └── voicedesk_driver.c
│
├── include/
│   ├── command_executor.hpp
│   ├── command_parser.hpp
│   ├── device_driver_client.hpp
│   ├── file_manager.hpp
│   ├── process_manager.hpp
│   ├── safety_engine.hpp
│   ├── system_monitor.hpp
│   └── voice_engine.hpp
│
├── src/
│   ├── command_executor.cpp
│   ├── command_parser.cpp
│   ├── device_driver_client.cpp
│   ├── file_manager.cpp
│   ├── main.cpp
│   ├── process_manager.cpp
│   ├── safety_engine.cpp
│   ├── system_monitor.cpp
│   └── voice_engine.cpp
│
├── tests/
│
└── third_party/
    └── whisper.cpp/
```

---

# 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++17 | Main application |
| Linux | Target operating system |
| Linux Kernel | Character device driver |
| whisper.cpp | Local speech recognition |
| GCC / G++ | C++ compilation |
| GNU Make | Build system |
| CMake | whisper.cpp build |
| Git | Version control |
| GitHub | Source-code hosting |
| PulseAudio | Linux audio capture |
| WSL2 | Development environment |

---

# 📋 Requirements

VoiceDesk requires a Linux environment with:

- GCC / G++
- GNU Make
- CMake
- Git
- Linux development tools
- Compatible Linux kernel development/source environment for the driver
- whisper.cpp dependencies
- Audio support for voice functionality

Verify the main development tools:

```bash
uname -a
g++ --version
git --version
cmake --version
make --version
```

---

# 📥 Installation

## 1. Clone the Repository

```bash
git clone https://github.com/SRIYA-BOSE/VoiceDesk.git
cd VoiceDesk
```

Initialize the whisper.cpp submodule:

```bash
git submodule update --init --recursive
```

---

## 2. Build whisper.cpp

Enter the whisper.cpp directory:

```bash
cd third_party/whisper.cpp
```

Configure:

```bash
cmake -B build -DWHISPER_SDL2=ON
```

Build:

```bash
cmake --build build -j1 --config Release
```

Return to the project root:

```bash
cd ../..
```

---

## 3. Add the Whisper Model

Place the required English Whisper model at:

```text
third_party/whisper.cpp/models/ggml-base.en.bin
```

The model is excluded from Git through `.gitignore`.

---

## 4. Build VoiceDesk

From the project root:

```bash
make
```

The executable will be generated at:

```text
build/voicedesk
```

---

# 🔧 Linux Character Driver Setup

## 1. Build the Driver

Enter the driver directory:

```bash
cd driver
```

Build:

```bash
make
```

The resulting module is:

```text
voicedesk_driver.ko
```

Return to the project root:

```bash
cd ..
```

> The driver must be built against a compatible Linux kernel development/source environment.

---

## 2. Load the Driver

```bash
sudo insmod driver/voicedesk_driver.ko
```

Verify the module:

```bash
lsmod | grep voicedesk
```

Verify the device:

```bash
ls -l /dev/voicedesk
```

Check kernel messages:

```bash
dmesg | grep voicedesk
```

---

## 3. Device Permissions

A udev rule can be configured as:

```text
KERNEL=="voicedesk", MODE="0660", GROUP="users"
```

Reload udev rules:

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Verify:

```bash
ls -l /dev/voicedesk
```

---

# ▶️ Running VoiceDesk

Start the application:

```bash
./build/voicedesk
```

Example keyboard interaction:

```text
VoiceDesk> open firefox
```

For voice interaction:

```text
VoiceDesk> voice
```

Then speak a supported command.

---

# 🧪 Testing

## System Information

```text
show system information
```

## Running Processes

```text
show processes
```

## List Files

```text
list files
```

## Create File

```text
create file test.txt
```

Verify:

```bash
ls -l test.txt
```

## Delete File

```text
delete file test.txt
```

Verify:

```bash
ls -l test.txt
```

## Open Application

```text
open firefox
```

## Close Application

```text
close firefox
```

## Help

```text
help
```

## Exit

```text
exit
```

---

# 🛡️ Safety Test

A key project demonstration is the safety layer.

Enter:

```text
rm -rf /
```

Expected:

```text
[Safety] Command blocked for security reasons.
```

The dangerous command should not be executed.

This demonstrates the safety path:

```text
User Input
    ↓
SafetyEngine
    ↓
BLOCK
```

---

# 🔍 Driver Verification

Check whether the driver is loaded:

```bash
lsmod | grep voicedesk
```

Check the device:

```bash
ls -l /dev/voicedesk
```

Check kernel logs:

```bash
dmesg | grep voicedesk
```

Typical driver messages include:

```text
voicedesk: driver loaded
voicedesk: device opened
voicedesk: received command: open firefox
voicedesk: device closed
```

These messages demonstrate the communication path between the VoiceDesk application and the Linux character driver.

---

# 🎬 Evaluation Demonstration

A short evaluation can demonstrate the complete system.

### Step 1 — Verify the Driver

```bash
lsmod | grep voicedesk
```

### Step 2 — Verify the Device

```bash
ls -l /dev/voicedesk
```

### Step 3 — Start VoiceDesk

```bash
./build/voicedesk
```

### Step 4 — Execute a Command

```text
open firefox
```

### Step 5 — Verify Kernel Communication

In another Linux terminal:

```bash
dmesg | tail -20
```

### Step 6 — Demonstrate Safety

```text
rm -rf /
```

Expected:

```text
[Safety] Command blocked for security reasons.
```

### Step 7 — Demonstrate Voice

```text
voice
```

Speak:

```text
Open Firefox
```

Complete pipeline:

```text
Voice
  ↓
Speech Recognition
  ↓
Safety Engine
  ↓
Command Parser
  ↓
Command Executor
  ↓
Device Driver Client
  ↓
/dev/voicedesk
  ↓
Linux Character Driver
  ↓
Kernel
```

---

# 🧠 Linux Concepts Demonstrated

## User Space

The C++ application handles:

- Voice recognition
- Command parsing
- Safety validation
- Application execution
- File operations
- Process management
- System monitoring
- Driver communication

## Kernel Space

The Linux character driver handles:

- Character-device registration
- Major/minor numbers
- File operations
- Kernel buffer management
- User/kernel memory transfer
- Mutex synchronization
- Device lifecycle
- Kernel logging

---

# 🏛️ Software Architecture Concepts

VoiceDesk demonstrates:

### Modular Architecture

Each major responsibility is implemented as a separate component.

### Separation of Concerns

Voice recognition, safety, parsing, execution, system monitoring, file management, and driver communication are separated.

### Encapsulation

C++ classes expose focused interfaces for individual subsystems.

### Defensive Programming

User input is validated before execution.

### Layered Architecture

```text
Input
  ↓
Recognition
  ↓
Safety
  ↓
Parsing
  ↓
Execution
  ↓
Driver Communication
  ↓
Kernel
```

### User-Space / Kernel-Space Separation

The application performs desktop operations in user space while the character driver demonstrates kernel-space communication.

---

# 🔬 Linux Character Driver Concepts

The driver demonstrates APIs and concepts including:

```text
module_init()
module_exit()

alloc_chrdev_region()
cdev_init()
cdev_add()

class_create()
device_create()

copy_to_user()
copy_from_user()

mutex
```

File operations include:

```text
open
read
write
release
```

---

# ❓ Why a Character Device?

A Linux character device provides a standard file-like interface between user-space applications and kernel-space drivers.

VoiceDesk uses:

```text
/dev/voicedesk
```

The C++ application can therefore communicate with the driver using standard Linux file operations:

```text
open()
read()
write()
close()
```

This provides a clear and demonstrable user-space ↔ kernel-space boundary.

---

# 🔐 Public Repository Security

This repository is intended to remain publicly accessible.

**Never commit sensitive information**, including:

```text
API keys
Access tokens
Passwords
Private SSH keys
Cloud credentials
.env files containing secrets
Authentication tokens
Private certificates
Personal credentials
```

Build artifacts and downloaded Whisper models are excluded through `.gitignore`.

Before pushing changes, review the staged content:

```bash
git status
git diff --cached
```

If a secret is accidentally added, remove it from the repository history and rotate the exposed credential immediately.

---

# ⚠️ Current Limitations

- Application control uses a predefined allowlist.
- Speech recognition requires whisper.cpp and a local model.
- Voice capture depends on the Linux audio environment.
- The character driver is primarily a kernel communication and educational component rather than a physical hardware driver.
- Driver compilation requires a compatible Linux kernel development/source environment.
- A Linux kernel ABI change may require rebuilding the driver.
- Speech recognition quality depends on the selected Whisper model and audio conditions.

---

# 🔮 Future Improvements

Potential improvements include:

- Direct PulseAudio/ALSA C++ audio capture
- Wake-word detection
- Multilingual speech recognition
- Additional desktop applications
- Graphical user interface
- Desktop notifications
- Driver event notifications
- `poll()` / `select()` support
- IOCTL-based driver commands
- Persistent command history
- Hardware microphone integration
- Advanced intent classification
- More Linux system-management capabilities

---

# 📊 Project Status

VoiceDesk currently provides:

- ✅ C++17 Linux desktop assistant
- ✅ Local speech recognition
- ✅ Natural-language command parsing
- ✅ Safety validation
- ✅ Controlled application management
- ✅ File management
- ✅ Process management
- ✅ System monitoring
- ✅ Custom Linux character device driver
- ✅ `/dev/voicedesk` device interface
- ✅ User-space ↔ kernel-space communication
- ✅ Kernel synchronization using mutex
- ✅ Modular software architecture
- ✅ GitHub documentation
- ✅ Driver documentation
- ✅ Installation documentation

---

# 📚 Documentation

Additional project documentation is available in:

```text
docs/
├── architecture.md
└── installation.md
```

Driver documentation:

```text
driver/
└── README.md
```

---

# 🌐 Repository

GitHub:

https://github.com/SRIYA-BOSE/VoiceDesk

---

# 👨‍💻 Project Summary

## VoiceDesk — Voice-Controlled Linux Desktop Assistant

VoiceDesk demonstrates the integration of:

```text
C++17
      +
Linux System Programming
      +
Speech Recognition
      +
Natural-Language Command Processing
      +
Command Safety
      +
Linux Character Device Driver
      +
User-Space / Kernel-Space Communication
```

The project is designed as an academic Linux systems project demonstrating how a modern voice interface can be combined with **C++ software architecture and Linux kernel programming concepts**.

---

## 📄 License

This project is intended for academic, educational, and demonstration purposes.
# 🎬 Evaluation Demonstration

A short evaluation can demonstrate the complete system.

### Step 1 — Verify the Driver

```bash
lsmod | grep voicedesk
```

### Step 2 — Verify the Device

```bash
ls -l /dev/voicedesk
```

### Step 3 — Start VoiceDesk

```bash
./build/voicedesk
```

### Step 4 — Execute a Command

```text
open firefox
```

### Step 5 — Verify Kernel Communication

In another Linux terminal:

```bash
dmesg | tail -20
```

### Step 6 — Demonstrate Safety

```text
rm -rf /
```

Expected:

```text
[Safety] Command blocked for security reasons.
```

### Step 7 — Demonstrate Voice

```text
voice
```

Speak:

```text
Open Firefox
```

Complete pipeline:

```text
Voice
  ↓
Speech Recognition
  ↓
Safety Engine
  ↓
Command Parser
  ↓
Command Executor
  ↓
Device Driver Client
  ↓
/dev/voicedesk
  ↓
Linux Character Driver
  ↓
Kernel
```

---

# 🧠 Linux Concepts Demonstrated

## User Space

The C++ application handles:

- Voice recognition
- Command parsing
- Safety validation
- Application execution
- File operations
- Process management
- System monitoring
- Driver communication

## Kernel Space

The Linux character driver handles:

- Character-device registration
- Major/minor numbers
- File operations
- Kernel buffer management
- User/kernel memory transfer
- Mutex synchronization
- Device lifecycle
- Kernel logging

---

# 🏛️ Software Architecture Concepts

VoiceDesk demonstrates:

### Modular Architecture

Each major responsibility is implemented as a separate component.

### Separation of Concerns

Voice recognition, safety, parsing, execution, system monitoring, file management, and driver communication are separated.

### Encapsulation

C++ classes expose focused interfaces for individual subsystems.

### Defensive Programming

User input is validated before execution.

### Layered Architecture

```text
Input
  ↓
Recognition
  ↓
Safety
  ↓
Parsing
  ↓
Execution
  ↓
Driver Communication
  ↓
Kernel
```

### User-Space / Kernel-Space Separation

The application performs desktop operations in user space while the character driver demonstrates kernel-space communication.

---

# 🔬 Linux Character Driver Concepts

The driver demonstrates APIs and concepts including:

```text
module_init()
module_exit()

alloc_chrdev_region()
cdev_init()
cdev_add()

class_create()
device_create()

copy_to_user()
copy_from_user()

mutex
```

File operations include:

```text
open
read
write
release
```

---

# ❓ Why a Character Device?

A Linux character device provides a standard file-like interface between user-space applications and kernel-space drivers.

VoiceDesk uses:

```text
/dev/voicedesk
```

The C++ application can therefore communicate with the driver using standard Linux file operations:

```text
open()
read()
write()
close()
```

This provides a clear and demonstrable user-space ↔ kernel-space boundary.

---

# 🔐 Public Repository Security

This repository is intended to remain publicly accessible.

**Never commit sensitive information**, including:

```text
API keys
Access tokens
Passwords
Private SSH keys
Cloud credentials
.env files containing secrets
Authentication tokens
Private certificates
Personal credentials
```

Build artifacts and downloaded Whisper models are excluded through `.gitignore`.

Before pushing changes, review the staged content:

```bash
git status
git diff --cached
```

If a secret is accidentally added, remove it from the repository history and rotate the exposed credential immediately.

---

# ⚠️ Current Limitations

- Application control uses a predefined allowlist.
- Speech recognition requires whisper.cpp and a local model.
- Voice capture depends on the Linux audio environment.
- The character driver is primarily a kernel communication and educational component rather than a physical hardware driver.
- Driver compilation requires a compatible Linux kernel development/source environment.
- A Linux kernel ABI change may require rebuilding the driver.
- Speech recognition quality depends on the selected Whisper model and audio conditions.

---

# 🔮 Future Improvements

Potential improvements include:

- Direct PulseAudio/ALSA C++ audio capture
- Wake-word detection
- Multilingual speech recognition
- Additional desktop applications
- Graphical user interface
- Desktop notifications
- Driver event notifications
- `poll()` / `select()` support
- IOCTL-based driver commands
- Persistent command history
- Hardware microphone integration
- Advanced intent classification
- More Linux system-management capabilities

---

# 📊 Project Status

VoiceDesk currently provides:

- ✅ C++17 Linux desktop assistant
- ✅ Local speech recognition
- ✅ Natural-language command parsing
- ✅ Safety validation
- ✅ Controlled application management
- ✅ File management
- ✅ Process management
- ✅ System monitoring
- ✅ Custom Linux character device driver
- ✅ `/dev/voicedesk` device interface
- ✅ User-space ↔ kernel-space communication
- ✅ Kernel synchronization using mutex
- ✅ Modular software architecture
- ✅ GitHub documentation
- ✅ Driver documentation
- ✅ Installation documentation

---

# 📚 Documentation

Additional project documentation is available in:

```text
docs/
├── architecture.md
└── installation.md
```

Driver documentation:

```text
driver/
└── README.md
```

---

# 🌐 Repository

GitHub:

https://github.com/SRIYA-BOSE/VoiceDesk

---

# 👨‍💻 Project Summary

## VoiceDesk — Voice-Controlled Linux Desktop Assistant

VoiceDesk demonstrates the integration of:

```text
C++17
      +
Linux System Programming
      +
Speech Recognition
      +
Natural-Language Command Processing
      +
Command Safety
      +
Linux Character Device Driver
      +
User-Space / Kernel-Space Communication
```

The project is designed as an academic Linux systems project demonstrating how a modern voice interface can be combined with **C++ software architecture and Linux kernel programming concepts**.

---

## 📄 License

This project is intended for academic, educational, and demonstration purposes.
