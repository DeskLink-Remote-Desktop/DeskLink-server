# DeskLink Server

This repository contains the source code for the DeskLink server. DeskLink is a remote desktop protocol designed for 
sending and receiving actions to be executed on a target computer. This server listens for incoming 
connections, authenticates them, and processes the received actions.

## Features

- **TCP/IP based:** Uses TCP/IP for reliable data transmission.
- **TLS Encryption:**  Secures communication using TLS encryption.
- **User Authentication:** Requires explicit user acceptance of incoming connections.
- **Robust Error Handling:** Includes comprehensive error handling and security checks to prevent unauthorized access
  and mitigate potential attacks.
- **Cross-Platform Compatibility (Planned):**  The design aims for cross-platform compatibility, though currently only
  Windows is supported.

## Build Instructions

This project uses a Makefile for building. To build the server:

1. **Prerequisites:**
   Ensure you have a C++ compiler (like G++), and Make.
2. **Compilation:**
   Navigate to the project's root directory and run this command:
   
   ```shell
   make
   ```
   
   This will create the executable in the `bin` directory.
3. **Running:**
   Execute the server using that command:
   
   ```shell
   ./bin/DeskLink_server.exe [port]
   ```
   
   (replace `[port]` with the desired port number, or omit for the default port 5055)

## Usage

The server listens for incoming connections on the specified port. When a connection request is received, it prompts
the user to accept or reject the connection. Upon acceptance, a handshake is performed, and the server begins
processing incoming data packets. Each packet contains an action code and associated data, which the server interprets
and executes on the local system.

## Protocol Specification

Detailed protocol specifications can be found in [`docs/protocol_specifications.md`](./docs/protocol_specifications.md).

## License

This project is licensed under the [GNU General Public License v2.0 only](http://gnu.org/licenses/old-licenses/gpl-2.0), see the [`LICENSE`](./LICENSE) file for
details.

