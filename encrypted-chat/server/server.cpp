#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "server.h"

using namespace std;

Server::Server() {
	this->initServer();
}

Server::~Server() {
	cout << "destructing server" << endl;
}

void Server::initServer() {
	cout << "hello world" << endl;
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
		cerr << "server.cpp::main(): socket() threw an error with code: " << strerror(errno) << endl;

		throw runtime_error("server.cpp::main(): Unable to allocate file descriptor when attempting to initialize socket");
	}
	else {
		cout << "Connection established, file descriptor: " << serverSocket << endl;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(serverSocket, 5);

	// Creates a new socket and returns a unique FD to communicate with an individual client
	cout << "server.cpp::main(): Waiting for client connection... " << endl;

	int clientSocket = accept(serverSocket, nullptr, nullptr);

	// Receive data from client
	while (1) {
		char buffer[1024] = {0};
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == 0) {
			cout << "Client disconnected!" << endl;
			break;
		}
		if (bytesReceived < 0) {
			cout << "Error receiving data from client!" << endl;
			break;
		}
		// Successfully received data from client
		cout << "Client sent: " << buffer << endl;
	}

	close(serverSocket);
}