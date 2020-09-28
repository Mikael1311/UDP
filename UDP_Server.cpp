/*  Christopher Patrone
    Network Design
    Phase 1
    This program is written in C++, and the goal is to create a UDP server and client that
    can send messages back and forth.

    This file is for the UDP server code.

    REFERENCES:

    [1]  Kelly, Sloan. Sloan Kelly. "Starter UDP Server and Client in C++". 
         Youtube. 2017. URL: https://www.youtube.com/watch?v=uIanSvWou1M

    [2]  mattew99g. Guided Hacking. "C++ Winsock Networking Tutorial - Introduction".
         Youtube. 2019. URL: https://www.youtube.com/watch?v=GiOgWfmWzPY

    [3]  Tougher, Rob. Linux Gazette. "Linux Socket Programming in C++". 
         tldp.org. 2002. URL: https://tldp.org/LDP/LG/issue74/tougher.html#4

    [4]  Hall, Brian. Beej's Guide to Network Programming. "Using Internet Sockets". 
         beej.us. 2019. URL: https://beej.us/guide/bgnet/html/#windows

*/

#include <iostream>
#include <ws2tcpip.h>
#include <sys/sendfile.h>

#pragma comment (lib, "ws2_32.lib")  // Add winsock library

using namespace std;


void main()
{
    // Initialize winsock
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wstart = WSAStartup(version, &data);

    if (wstart != 0) {
        cout << "ERROR: Failure to start up winsock " << wstart << endl;  // ERROR, winsock cannot start up
        return;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);  // DGRAM -> Datagram -> UDP Socket
    sockaddr_in sockAdd;
    sockAdd.sin_addr.S_un.S_addr = ADDR_ANY;  // Assign ip address to socket
    sockAdd.sin_family = AF_INET;  // Address family
    sockAdd.sin_port = htons(54000);  // Convert to big endian
    // htons -> Host To Network Short

    // Check for errors in binding socket
    if (bind(sock, (sockaddr*)&sockAdd, sizeof(sockAdd)) == SOCKET_ERROR) {
        cout << "ERROR: Failure to bind socket " << WSAGetLastError() << endl; 
        return;
    }

    else {
        cout << "Server Ready to Recieve from Client" << endl;
    }

    // Look for client connection information
    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, sizeof(client, clientLength));  // Zero client information

    // Create buffer to hold client information
    char buffer[1024];

    // Loop and recieve information from client
    while (true) {
        ZeroMemory(buffer, 1024);  // Zero the buffer

        // Wait for message from the client
        int bytesRec = recvfrom(sock, buffer, 1024, 0, (sockaddr*)&client, &clientLength);

        }

        // Client message information
        char clientIP[256];
        ZeroMemory(clientIP, 256);  // Zero buffer for client message

        // Converts from internet address in binary format to standard text format
        inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

        // Output message received from client and the client's IP address
        cout << "Message received from " << clientIP << " : " << buffer << endl;


        /// Send message back to client, confirming message has been received
        int sendOkay = sendto(sock, buffer, 1024, 0, (sockaddr*)&client, clientLength);

        }
    }

    // Close socket
    closesocket(sock);

    // Shut down winsock
    WSACleanup();

    return;
}
