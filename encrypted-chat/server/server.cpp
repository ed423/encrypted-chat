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

int main()
{
	//------------------------------------------------------------------------------
	// Initialize socket and block until a client connects
	//------------------------------------------------------------------------------

	// Create TCP socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		std::cerr << "server.cpp::main(): socket() threw an error with code: " << strerror(errno) << std::endl;

		throw std::runtime_error("server.cpp::main(): Unable to allocate file descriptor when attempting to initialize socket");
	}
	else {
		std::cout << "server.cpp::main(): Connection established, file descriptor: " << serverSocket << std::endl;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	// cout << "SERVER_PORT: " << SERVER_PORT << endl;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
	listen(serverSocket, 5);

	// Creates a new socket and returns a unique FD to communicate with an individual client
	std::cout << "server.cpp::main(): Waiting for client connection... " << std::endl;

	int serverAddressLen = sizeof(struct sockaddr_in);
	// int clientSocket = accept(serverSocket, nullptr, nullptr);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress,(socklen_t *)&serverAddressLen);
	if (clientSocket < 0) {
		perror("Accept connection");
	}
	std::cout << "server.cpp::main(): client accepted" << std::endl;

	//--------------------------------------
	// Receive data from client and echos
	//--------------------------------------
	while (1) {
        // cout << "server.cpp::main(): while" << endl;
		char buffer[1024] = {0};
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == 0) {
			std::cout << "server.cpp::main(): Client disconnected!" << std::endl;
			break;
		}
		if (bytesReceived < 0) {
			// Received no data (client may not be connected yet), try again
			continue;
		}
		// Successfully received data from client
		std::cout << "server.cpp::main(): Client sent: " << buffer << std::endl;
		// Echo data back to client
		send(clientSocket, buffer, sizeof(buffer), 0);
	}

	close(serverSocket);
}

// //==========
// // test
// //==========
// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>



// const int SERVER_PORT = 12345; // Port number for the server

// int main() {
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(SERVER_PORT);

// 	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//     if (listen(serverSocket, 5) < 0) {
//         perror("Listen failed");
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Server listening on port " << SERVER_PORT << std::endl;

//     int clientSocket;
//     sockaddr_in clientAddr;
//     socklen_t clientAddrLen = sizeof(clientAddr);

//     clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
//     if (clientSocket < 0) {
//         perror("Accept failed");
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

//     close(clientSocket);
//     close(serverSocket);

//     return 0;
// }
