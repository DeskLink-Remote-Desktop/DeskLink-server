#include <iostream>
#include <vector>

#include "actions.hpp"
#include "errors.hpp"
#include "metadata.hpp"
#include "data_transmission.hpp"


int main(const int argc, char **argv) {
	const std::vector<std::string> args(argv, argv + argc);
	uint16_t port = 0;

	std::cout << NAME_STR " v" VERSION << std::endl << std::endl;

	if (argc > 2) {
		error("Too many arguments");
	} else if (argc == 2) {
		try {
			port = std::stol(args[1]);
		} catch ([[maybe_unused]] const std::exception &e) {
			error("Invalid port: \"" + args[1] + "\".");
		}
	}

	if (!port && argc != 2) {
		error("No port provided. Use default " + DEFAULT_PORT_STR + ".", ErrorLevel::Warning);
		port = DEFAULT_PORT;
	}

	if (port < 1024) {
		error("Invalid port: " + std::to_string(port) + "; Port must be greater than 1023. "
			"Use default " + DEFAULT_PORT_STR + ".", ErrorLevel::Warning);
		port = DEFAULT_PORT;
	}

	std::vector<std::string> addresses;

	try {
		addresses = setup_connection(port);
	} catch (connection_error &e) {
		error(e.what());
	}

	std::cout << "Connected to " << addresses.at(1) << " (remote) at " << addresses.at(0) << " (local)" << std::endl;

	while (true) {
		data received_data;

		try {
			received_data = get_data();
		} catch (connection_error &e) {
			error(e.what());
			exit(EXIT_FAILURE);
		}

		switch (received_data.action) {
			case 0:
				if (received_data.data > (1 << 16) - 1) {
					error("Bad key code number received from client. "
						"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
				}

				keydown(received_data.data);
				std::cout << "Keydown key " << received_data.data << std::endl;
				break;
			case 1:
				if (received_data.data > (1 << 16) - 1) {
					error("Bad key code number received from client. "
						"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
				}

				keyup(received_data.data);
				std::cout << "Keyup key " << received_data.data << std::endl;
				break;
			case 2: {
				const uint16_t x = received_data.data >> 16;
				const uint16_t y = received_data.data;

				int max_x;
				int max_y;

				#ifdef _WIN32

				max_x = GetSystemMetrics(SM_CXSCREEN);
				max_y = GetSystemMetrics(SM_CYSCREEN);

				#else
				error("Can't validate mouse coordinates: Unhandled platform.", ErrorLevel::Warning);
				#endif

				if (x > max_x || y > max_y) {
					error("Bad coordinates received from client. "
						"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
				}

				mouse_move(x, y);
				std::cout << "Move mouse to " << x << ", " << y << std::endl;
				break;
			}
			case 3:
				if (received_data.data > 1) {
					error("Bad scroll direction received from client. "
						"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
				}

				scroll(received_data.data);
				std::cout << "Scroll of " << received_data.data << std::endl;
				break;
			default:
				error("Bad action received from client. "
					"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
				break;
		}
	}
}
