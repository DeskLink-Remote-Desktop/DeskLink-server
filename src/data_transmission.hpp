#ifndef DATA_TRANSMISSION_HPP
#define DATA_TRANSMISSION_HPP

#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#define SOCK_T SOCKET
#elif defined(__linux__)
#include <arpa/inet.h>
#elif defined(__APPLE__)
#include <arpa/inet.h>
#endif


#pragma pack(push, 1)
typedef struct {
	uint8_t action; // 0: Keydown 1: Keyup  2: Mouse move           3: Scroll
	uint32_t data;  //    Keycode    Keycode   Coordinates ({x, y})    Direction (up: 0, down: 1)

	void to_network() {
		data = htonl(data);
	}

	void from_network() {
		data = ntohl(data);
	}
} data;
#pragma pack(pop)

class connection_error final : public std::exception {
	std::string msg;
public:
	explicit connection_error(const std::string &msg) : msg("An error occurred in the connection: " + msg) {}

	[[nodiscard]] const char *what() const noexcept override {
		return msg.c_str();
	}
};

std::vector<std::string> setup_connection(uint16_t port);
data get_data();
void cleanup();


#endif