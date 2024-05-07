#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "../server/server.h"

using namespace std;

int main() {
    //------------------------------------------------------------------------------
    // Initialize client socket and connect to server
    //------------------------------------------------------------------------------
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    const char* message = "hello world";
    send(clientSocket, message, strlen(message), 0);

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
        cout << "Server says: " << buffer << endl;

        // Read in user input from command line and store in variable
        string message;
        getline(cin, message);
        char* convertedMessage = message.data();
        send(clientSocket, convertedMessage, strlen(convertedMessage), 0);
    }

    close(clientSocket);
}