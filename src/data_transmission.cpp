#include "data_transmission.hpp"
#include "errors.hpp"
#include "metadata.hpp"

#include <algorithm>
#include <iostream>


SOCK_T client_sock;
SOCK_T listen_sock;

void handshake();

void setup_tls() {
	#ifdef _WIN32

	#elif defined(__linux__)

	#elif defined(__APPLE__)

	#else
	#error Unhandled platform
	#endif
}

// Returns: { Local IP address with port, Remote IP address with port }
std::vector<std::string> setup_connection(const uint16_t port) {
	int error_code;
	std::string local_addr;
	std::string remote_addr;

	#ifdef _WIN32

	WSADATA wsa_data;
	error_code = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error_code) throw connection_error("Startup failed: " + std::to_string(WSAGetLastError()));

	listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in server_addr{};

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	error_code = bind(listen_sock, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
	if (error_code) throw connection_error("Couldn't bind to the port: " + std::to_string(WSAGetLastError()));

	sockaddr_in local_addr_in{};
	int addr_len = sizeof(local_addr_in);
	error_code = getsockname(listen_sock, reinterpret_cast<sockaddr *>(&local_addr_in), &addr_len);
	if (error_code) throw connection_error("Couldn't retrieve the local address: "
		+ std::to_string(WSAGetLastError()));
	local_addr = inet_ntoa(local_addr_in.sin_addr) + std::string(":")
		+ std::to_string(ntohs(local_addr_in.sin_port));

	error_code = listen(listen_sock, SOMAXCONN);
	if (error_code) throw connection_error("Couldn't listen to the port: " + std::to_string(WSAGetLastError()));

	std::cout << "Waiting for connection at " << local_addr << std::endl;

	while (true) {
		sockaddr_in remote_addr_in{};
		int remote_addr_len = sizeof(remote_addr_in);
		client_sock = accept(listen_sock, reinterpret_cast<sockaddr *>(&remote_addr_in), &remote_addr_len);
		setup_tls();

		u_long mode = 1;
		ioctlsocket(client_sock, FIONBIO, &mode);

		if (client_sock != INVALID_SOCKET) {
			remote_addr = inet_ntoa(remote_addr_in.sin_addr) + std::string(":")
				+ std::to_string(ntohs(remote_addr_in.sin_port));
		} else {
			throw connection_error("Couldn't retrieve remote address: " + std::to_string(WSAGetLastError()));
		}

		std::string accept_connection;
		std::cout << "Receiving connection request from " << remote_addr
				  << ". Do you want to accept it? [y/N]" << std::endl;

		std::getline(std::cin, accept_connection);
		std::transform(accept_connection.begin(), accept_connection.end(), accept_connection.begin(),
			[](const unsigned char c){ return std::tolower(c); });

		if (accept_connection == "y") {
			if (client_sock == INVALID_SOCKET) {
				throw connection_error("Couldn't accept the connection: " + std::to_string(WSAGetLastError()));
			}

			// Workaround for specification 2.1§2
			char buff[1];
			if (const int result = recv(client_sock, buff, sizeof(buff), 0); result > 0) {
				error("Received data before accepting the connection. "
					"Shutting down the server for security reasons.", ErrorLevel::CriticalError);
			}

			mode = 0;
			ioctlsocket(client_sock, FIONBIO, &mode);
			handshake();
			std::cout << "Connection accepted from " << remote_addr << std::endl;
			break;
		}

		closesocket(client_sock);
		mode = 0;
		ioctlsocket(client_sock, FIONBIO, &mode);
		std::cout << "Connection refused from " << remote_addr << std::endl;
	}

	#elif defined(__linux__)

	#elif defined(__APPLE__)

	#else
	#error Unhandled platform
	#endif

	atexit(cleanup);
	signal(SIGINT, [](int) {
		std::cout << "Interrupted by user." << std::endl;
		exit(EXIT_FAILURE);
	});
	return {local_addr, remote_addr};
}

void handshake() {
	const std::string handshake_str = std::string() + PROTOCOL_NAME " v" PROTOCOL_VERSION "\n";
	const int handshake_str_sz = static_cast<int>(handshake_str.size());

	#ifdef _WIN32

	send(client_sock, handshake_str.c_str(), handshake_str_sz, 0);

	#elif defined(__linux__)

	#elif defined(__APPLE__)

	#else
	#error Unhandled platform
	#endif
}

data get_data() {
	#ifdef _WIN32

	char buff[sizeof(data)];

	int received_bytes = recv(client_sock, buff, sizeof(buff), 0);

	if (received_bytes == sizeof(data)) {
		const auto received_data = reinterpret_cast<data *>(buff);
		received_data->from_network();
		return *received_data;
	}

	if (received_bytes == SOCKET_ERROR) {
		throw connection_error("Socket error: " + std::to_string(WSAGetLastError()));
	}

	if (received_bytes == 0) {
		throw connection_error("Connection closed");
	}

	if (static_cast<size_t>(received_bytes) < sizeof(data)) {
		while (static_cast<size_t>(received_bytes) < sizeof(data)) {
			received_bytes += recv(client_sock, buff + received_bytes, sizeof(buff) - received_bytes, 0);

			if (received_bytes == SOCKET_ERROR) {
				throw connection_error("Socket error: " + std::to_string(WSAGetLastError()));
			}

			if (received_bytes == 0) {
				throw connection_error("Connection closed");
			}
		}
	}

	throw connection_error("Malformed data received");

	#elif defined(__linux__)

	#elif defined(__APPLE__)

	#else
	#error Unhandled platform
	#endif
}

void cleanup() {
	#ifdef _WIN32

	closesocket(listen_sock);
	closesocket(client_sock);
	WSACleanup();

	#elif defined(__linux__)

	#elif defined(__APPLE__)

	#else
	#error Unhandled platform
	#endif

	std::cout << "Cleanup done." << std::endl;
}
