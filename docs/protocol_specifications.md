# DeskLink Protocol Specifications

The DeskLink Protocol ("the protocol") is a powerful protocol for remote desktop applications and utilities, to send
and receive the actions to execute on the target computer ("the computer").

## 1. Connection

- Base protocol: TCP/IP
- Encryption: TLS

The protocol uses TCP/IP for data transmission over Internet connection, and uses TLS for data encryption as the
transmitted data is actions that are executed directly on the computer.

To connect, the protocol needs an IP address (IPv4 or IPv6) and a port, by default the port 5055.

## 2. Authentication

The Server application ("the server") has to ask the user directly to accept or reject the connection. For that, the
server needs to show the IP address and port of the connection request, and ask the user to accept or reject it.  If
the user accepts, the connection is established; otherwise, it's refused.

The Client application ("the client") should show the IP address and port of the device with the client, aside of
instructions about accepting the connection request on the server.

### 2.1 Handshake

After the user accept the connection on the server, the server should send then handshake:

```c++
"DeskLink v[version]\n"
```

For the time between the connection request and the receiving of the handshake, the client shouldn't send any data,
otherwise the server will stop for security reasons (preventing unauthorized access).

## 3. Data

The data is constituted of 5 bytes:

- byte 0: Action code
- bytes 1-5: Additional data

Here is a chart of the action codes and data:

| Action code | Data                                | Action              | Description                                              |
|------------:|-------------------------------------|---------------------|----------------------------------------------------------|
|         `0` | `uint32_t keycode`                  | `keydown(keycode)`  | Keydown the key with the keycode `keycode`.              |
|         `1` | `uint32_t keycode`                  | `keyup(keycode)`    | Keyup the key with the keycode `keycode`.                |
|         `2` | `(uint16_t x) << 16 + (uint16_t y)` | `mouse_move(x, y)`  | Move the mouse to the coordinates (`x`, `y`)             |
|         `3` | `uint32_t direction`                | `scroll(direction)` | Scroll in the direction `direction` (`0`: up, `1`: down) |

If any received data doesn't match the expected formats, then the server will stop for security reasons
(preventing man-in-the-middle attacks).

### 3.1 Keycodes

The keycodes are JavaScript keycodes. Certain unused keycodes have been remapped to handle mouse buttons:

| Keycode | Mouse button                  |
|--------:|-------------------------------|
|     `1` | Mouse button 1 (left click)   |
|     `2` | Mouse button 2 (right click)  |
|     `3` | Mouse button 3 (middle click) |

### 3.2 Data structure

The data structure can be represented as a C/C++ `struct`:

```c++
#pragma pack(push, 1)
typedef struct {
	uint8_t action;
	uint32_t data;

	void to_network() {
		data = htonl(data);
	}

	void from_network() {
		data = ntohl(data);
	}
} data;
#pragma pack(pop)
```

## 4. URL protocol

The protocol can be represented as a URL of the scheme:

```
desklink://target_ip[:port]
```

If the port isn't provided, it should be defaulted to 5055.

This URL protocol would launch the client pre-connected to `target_ip` at port `port`.
