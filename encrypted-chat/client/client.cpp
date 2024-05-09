#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <thread>

#include "../server/server.h"

using namespace std;

//------------------------------------------------------------------------------
// Receive data from server
//------------------------------------------------------------------------------
void recvData(int clientSocket) {
    while (1) {
        char buffer[1024] = {0};
	    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

	    if (bytesReceived == 0) {
		    cout << "client.cpp::recvData(): Server disconnected!" << endl;
		    break;
	    }

	    if (bytesReceived < 0) {
	    	cout << "client.cpp::recvData(): Error connecting to server!" << endl;
    		break;
    	}

        cout << "client.cpp::recvData(): Server says: " << buffer << endl;
    }
}

//------------------------------------------------------------------------------
// Read in user input from command line and send to server
//------------------------------------------------------------------------------
void sendData(int clientSocket) {
    while (1) {
        string message;
        getline(cin, message);
        const char* convertedMessage = message.data();
        send(clientSocket, convertedMessage, strlen(convertedMessage), 0);
    }
}

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

    // Spawn two separate threads for reading and writing
    thread recvThread(recvData, clientSocket);
    thread sendThread(sendData, clientSocket);

    sendThread.join();

    // Detach as recvThread might block, but we still want to be able to send even if recvThread blocks
    recvThread.detach();

    close(clientSocket);
}