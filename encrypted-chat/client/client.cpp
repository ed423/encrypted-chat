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

// const int SERVER_PORT = 12345; // Port number for the server

// using namespace std;
//------------------------------------------------------------------------------
// Receive data from server
//------------------------------------------------------------------------------
void recvData(int clientSocket) {
    while (1) {
        char buffer[1024] = {0};
	    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

	    if (bytesReceived == 0) {
		    std::cout << "client.cpp::recvData(): Server disconnected!" << std::endl;
		    break;
	    }

	    if (bytesReceived < 0) {
	    	std::cout << "client.cpp::recvData(): Error connecting to server!" << std::endl;
    		break;
    	}

        std::cout << "client.cpp::recvData(): Server says: " << buffer << std::endl;
    }
}

//------------------------------------------------------------------------------
// Read in user input from command line and send to server
//------------------------------------------------------------------------------
void sendData(int clientSocket) {
    while (1) {
        std::string message;
        getline(std::cin, message);
        const char* convertedMessage = message.data();
        send(clientSocket, convertedMessage, sizeof(convertedMessage), 0);
    }
}

/// <#Description#>
int main() {
    //------------------------------------------------------------------------------
    // Initialize client socket and connect to server
    //------------------------------------------------------------------------------
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Create socket error");
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // cout << "SERVER_PORT: " << SERVER_PORT << endl;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    // cout << "CONNECTED" << endl;

    // Spawn two separate threads for reading and writing
    std::thread recvThread(recvData, clientSocket);
    std::thread sendThread(sendData, clientSocket);

    sendThread.join();

    // Detach as recvThread might block, but we still want to be able to send even if recvThread blocks
    recvThread.detach();

    close(clientSocket);
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

// // using namespace std;

// const int SERVER_PORT = 12345; // Port number for the server

// int main() {
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (clientSocket == -1) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(SERVER_PORT);
//     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of the server, in this case, localhost

//     if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
//         perror("Connection failed");
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Connected to server" << std::endl;

//     close(clientSocket);

//     return 0;
// }
