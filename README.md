# VoiceDesk

## Voice-Controlled Linux Desktop Assistant

VoiceDesk is a **C++17 voice-controlled Linux desktop assistant** designed to demonstrate the integration of **speech recognition, Linux system programming, command safety, process/file management, and a custom Linux character device driver**.

The system allows users to control selected desktop operations using natural-language commands through either keyboard input or voice input.

---

## ✨ Features

- 🎙️ Voice command recognition using **whisper.cpp**
- 🖥️ Linux desktop application control
- 🧠 Natural-language command parsing
- 🛡️ Dangerous-command safety validation
- 📂 File management
- ⚙️ Process management
- 📊 System information monitoring
- 🔌 Custom Linux character device driver
- 🔄 User-space ↔ kernel-space communication
- 🔐 Controlled application allowlist
- 🧩 Modular C++ architecture
- 🐧 Linux system programming concepts

---

# 1. Project Overview

Traditional desktop interaction requires users to manually operate applications, files, and system utilities.

VoiceDesk introduces a voice-driven interaction layer where commands such as:

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
