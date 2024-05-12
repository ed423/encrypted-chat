#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <thread>
#include <chrono>

#include "server.h"

// using namespace std;
// const int SERVER_PORT = 7000; // Port number for the server

Server::Server() {
	this->initServer();
}

Server::~Server() {
	std::cout << "destructing server" << std::endl;
}

void Server::initServer() {
	std::cout << "hello world" << std::endl;
}


int main() {
	//------------------------------------------------------------------------------
	// Initialize socket and block until a client connects
	//------------------------------------------------------------------------------

	// create a socket

	// bind - a socket involves associating a specific address (IP address and port number) with a socket

	// initialize to handle inputs from server_socketfd
	// listen
	// FD_ZERO
	// FD_SET

	// poll for clients and requests
	// while(1) {
		// ...
		// select() <- research what this does

		// for loop


			// check if activity is on the server's socket fd = connection request
			// handle requests
			// connection request handler
				// we accept the client here and add the clients fd to the descriptor set

			// else if (...)
				// messages... POO!

			// poohaps handle disconnection stuffs peehead
	// }
}


// int main()
// {
// 	//------------------------------------------------------------------------------
// 	// Initialize socket and block until a client connects
// 	//------------------------------------------------------------------------------

// 	// Create TCP socket
// 	int server_socketfd = socket(AF_INET, SOCK_STREAM, 0);

// 	if (server_socketfd == -1)
// 	{
// 		std::cerr << "server.cpp::main(): socket() threw an error with code: " << strerror(errno) << std::endl;

// 		throw std::runtime_error("server.cpp::main(): Unable to allocate file descriptor when attempting to initialize socket");
// 	}
// 	else {
// 		std::cout << "server.cpp::main(): Connection established, file descriptor: " << server_socketfd << std::endl;
// 	}

// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(SERVER_PORT);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;
// 	// cout << "SERVER_PORT: " << SERVER_PORT << endl;

// 	bind(server_socketfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	

// 	// Create a connection queue and initialize readfds to handle input from server_sockfd.
// 	listen(server_socketfd, 5);

// 	// Creates a new socket and returns a unique FD to communicate with an individual client
// 	std::cout << "server.cpp::main(): Waiting for client connection... " << std::endl;

// 	int serverAddressLen = sizeof(struct sockaddr_in);
// 	// int clientSocket = accept(server_socketfd, nullptr, nullptr);
// 	int clientSocket = accept(server_socketfd, (struct sockaddr *)&serverAddress,(socklen_t *)&serverAddressLen);
// 	if (clientSocket < 0) {
// 		perror("Accept connection");
// 	}
// 	std::cout << "server.cpp::main(): client accepted" << std::endl;

// 	//--------------------------------------
// 	// Receive data from client and echos
// 	//--------------------------------------
// 	while (1) {
//         // cout << "server.cpp::main(): while" << endl;
// 		char buffer[1024] = {0};
// 		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
// 		if (bytesReceived == 0) {
// 			std::cout << "server.cpp::main(): Client disconnected!" << std::endl;
// 			break;
// 		}
// 		if (bytesReceived < 0) {
// 			// Received no data (client may not be connected yet), try again
// 			continue;
// 		}

// 		// Successfully received data from client
// 		std::cout << "server.cpp::main(): Client sent: " << buffer << std::endl;
// 		// Echo data back to client
// 		send(clientSocket, buffer, sizeof(buffer), 0);
// 	}

// 	close(server_socketfd);
// }
