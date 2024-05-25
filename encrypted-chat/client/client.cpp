#include <iostream>
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <thread>


#include "../shared/util.h"
#include "../shared/packet_protocol.h"


// Bad, fix later
#include "../shared/packet_protocol.cpp"


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
       char* stringAsChar = new char[message.size() + 1];
       std::strcpy(stringAsChar, message.c_str());


       uint8_t *messageToSend = reinterpret_cast<uint8_t*>(stringAsChar);


       PacketProtocol packetToSend{999999, 5, 0, static_cast<uint16_t>(message.length()), messageToSend};
       send(clientSocket, packetToSend.getPkt(), MAX_PACKET_SIZE, 0);
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


   int connection = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
   if (connection == 0) {
       std::cout << "CONNECTED" << std::endl;
   } else {
       std::cout << "FAILED TO CONNECT TO SERVER" << std::endl;
   }


   // Spawn two separate threads for reading and writing
   std::thread recvThread(recvData, clientSocket);
   std::thread sendThread(sendData, clientSocket);


   sendThread.join();


   // Detach as recvThread might block, but we still want to be able to send even if recvThread blocks
   recvThread.detach();


   close(clientSocket);
}
