## Overview

This repository demonstrates principles and methodologies behind common Windows 10 backdoor techniques. It provides educational examples to understand how backdoors can be implemented, maintained, and hidden within the Windows operating system.

Note: This code is intended only for educational purposes and should not be used for any unauthorized or illegal activities. Ensure that you have appropriate permissions when testing or using this code.

## Client-side shell communication
This code implements a Windows-based client that establishes a connection to a remote server for local testing purposes. It provides basic remote shell functionality, such as executing commands and maintaining persistence by adding the program to the Windows startup registry.

### Features:
- Remote Shell Access: The program connects to a remote server, allowing it to receive commands and execute them on the local machine.
- Command Handling: Supports basic commands like q (quit), cd (change directory), and persist (add the program to the Windows startup registry for persistence).
- Persistence: The persist command adds the program to the Windows registry, making it run automatically on startup.
- Stealth Mode: The program runs hidden from the user by minimizing the console window upon launch.

### Way of working:
- Socket Connection: The program establishes a TCP connection to the server using the specified IP (192.168.1.175) and port (50005).
- Command Handling: executes known/custom commands (q, cd <path>, persist) as defined and other commands are executed on the local machine via popen and returns the output to the server.
- Console Stealth: The program runs without a visible console window using FindWindowA to hide it from the user.

## Server-side shell communication
This TCP server allows basic communication with a client through a shell-like interface.
### Features:
- TCP Socket Setup: Server listens on IP 192.168.1.175 and port 50005.
- Interactive Shell: Commands are read from the terminal and sent to the client.
- Command Handling: Supports q (quit), cd (placeholder for future functionality), and persist (send predefined response to client).
- Data Exchange: Server sends commands to the client and prints the response.

### Way of working:
- Socket: Creates a socket, binds to an IP and port, and listens for client connections.
- Command Loop: Waits for user input, sends commands to the client, and prints the client’s response.

## Prerequisites
- mingw-w64

## Installation
```
cd Win10-backdoor
make
```

## Improvements
The following are ideas for improving the functionality, security, and compatibility of the backdoor project. While Microsoft Defender provides strong built-in protection for Windows 10 systems, these enhancements aim to ensure that the backdoor is more robust, stealthy, and compatible across various environments.
- [ ] **Handle Exiting Signals:** Implement handling for signals that would allow the program to gracefully exit. In your current code, you are manually closing the socket and calling WSACleanup() when the "q" command is received, which ends the program.
- [ ] **Every Second Command Gets Executed:** An issue has been noticed where sometimes every other command doesn't get executed. This is likely due to internal issues like bad network traffic or timing problems, but it's better to check it out to be sure.
- [ ] **Make Program Compatible with Linux-based OS:** Currently, the backdoor is Windows-centric, relying heavily on Windows-specific libraries (like WinSock and Windows API). The goal is to make the program compatible with Linux-type operating systems
- [ ] **Check for Memory Leaks:** While the backdoor is functional, it's important to ensure that all dynamically allocated memory is freed properly to avoid memory leaks
- [ ] **Exchange `strcmp` with `strncmp`:** The program currently uses `strcmp` for string comparison. This can be a potential security risk, as `strcmp` does not prevent buffer overflow issues. Replacing `strcmp` with `strncmp` will limit the number of characters compared, providing a safer and more controlled string comparison.
- [ ] **Add an Enum and a Switch to Handle Commands:** To improve the readability and scalability of the code, we can define an enum to represent different commands. Using a switch statement to check for commands would make the code more modular, easier to extend, and easier to maintain.
