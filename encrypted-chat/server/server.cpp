#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <thread>
#include <chrono>

#include "../shared/util.h"

// using namespace std;
// const int SERVER_PORT = 7000; // Port number for the server

//------------------------------------------------------------------------------
// Reset flags in fd sets
//------------------------------------------------------------------------------
void reset_fd_sets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds, int server_socketfd) {
	FD_ZERO(read_fds);
	FD_ZERO(write_fds);
	FD_ZERO(except_fds);

	FD_SET(server_socketfd, read_fds); // add server's listen socket fd to fd set
}

//------------------------------------------------------------------------------
// Accept new connection on server's listen fd
//------------------------------------------------------------------------------
int accept_new_connection(int server_socketfd, sockaddr_in server_address, std::vector<int>& client_sockets) {
	int server_address_len = sizeof(struct sockaddr_in);
	int client_socketfd = accept(server_socketfd, (struct sockaddr *)&server_address,(socklen_t *)&server_address_len);
	if (client_socketfd < 0) {
		return -1;
	}
	// add client socket fd to list of known client socket fds
	client_sockets.push_back(client_socketfd);

	return client_socketfd;
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main() {
	fd_set read_fds;

	// unused for now, figure out what they do/if we need them later
	fd_set write_fds;
	fd_set except_fds;

	// create a socket
	int server_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// keeps track of connected clients' fds
	std::vector<int> client_sockets;

	// bind - a socket involves associating a specific address (IP address and port number) with a socket
	bind(server_socketfd, (struct sockaddr*)&server_address, sizeof(server_address));

	// initialize to handle inputs from server_socketfd
	listen(server_socketfd, 5);

	std::cout << "server.cpp::main(): Waiting for client connections..." << std::endl;

	// poll for clients and requests
	while(1) {
		reset_fd_sets(&read_fds, &write_fds, &except_fds, server_socketfd);

		// highest monitored fd number
		int max_fds = server_socketfd;

		// update max fd value by checking how many clients are connected
		for (int client_socketfd : client_sockets) {
            FD_SET(client_socketfd, &read_fds);
            if (client_socketfd > max_fds) {
                max_fds = client_socketfd;
            }
        }

		std::cout << "server.cpp::main(): current max fds: " << max_fds << std::endl;

		// ...
		// select() <- research what this does - monitors multiple file descriptors,
		// waits till one or more fds become ready for operations
		// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		// FD_SETSIZE = max num file descriptors in a fd_set
		int result = select(max_fds + 1, &read_fds, &write_fds, &except_fds, NULL);

		switch(result) {
			case -1:
				std::cerr << "server.cpp::main(): select() error! Returned: " << result << std::endl;
			case 0:
				std::cerr << "server.cpp::main(): select() timed out, returned " << result << std::endl;
			default:
				std::cout << "server.cpp::main(): select() returned: " << result << std::endl;
		}

		// incoming connection request to server
		if (FD_ISSET(server_socketfd, &read_fds) != 0) {
			std::cout << "server.cpp::main(): fd " << "connection request on server fd: " << server_socketfd << std::endl;
			int client_socketfd = accept_new_connection(server_socketfd, server_address, client_sockets);

			if (client_socketfd < 0) {
				std::cerr << "server.cpp::main(): Failed to accept() new connection";
			};

			std::cout << "server.cpp::main(): client accepted on server fd. Client socket fd is: " << client_socketfd << std::endl;
			continue;
		}

		// message from client, figure out which client is attempting to send a message
		for (int i = 0; i <= max_fds; i++) {
			if (FD_ISSET(i, &read_fds) != 0) {
				std::cout << "server.cpp::main(): client on fd " << i << " has sent a message" << std::endl;

				// receive data from client
				char buffer[1024] = {0};
				int bytesReceived = recv(i, buffer, sizeof(buffer), 0);

				if (bytesReceived == 0) {
					std::cout << "server.cpp::main(): Client disconnected!" << std::endl;

					// close disconnected client socket fd so we can reuse it later
					close(i);

					// remove disconnected client socket fd from list of connected client fds
					client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), i), client_sockets.end());
					continue;
				}

				if (bytesReceived < 0) {
					// Received no data (client may not be connected yet), try again
					continue;
				}

				// Successfully received data from client
				std::cout << "server.cpp::main(): Client sent: " << buffer << std::endl;
				// Echo data back to client
				send(i, buffer, sizeof(buffer), 0);
			}
		}
	}
}