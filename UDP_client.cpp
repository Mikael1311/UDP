/*  Christopher Patrone
    Network Design
    Phase 1
    This program is written in C++, and the goal is to create a UDP server and client that
    can send messages and back and forth.

    This file is for the UDP client code.

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
#include <string>

#pragma comment (lib, "ws2_32.lib")  // Add winsock library

using namespace std;


void main(int argc, char* argv[])  // Command line input
{
    // Initialize winsock
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wstart = WSAStartup(version, &data);

    if (wstart != 0) {
        cout << "ERROR: Failure to start up winsock " << wstart << endl;  // ERROR, winsock cannot start up
        return;
    }

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);  // Connect to port 54000
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    // Create a socket
    SOCKET sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Write out to socket
    string s(argv[1]);
    int sendOK = sendto(sock2, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));  // Send command line argument to server

    // Check for errors sending message
    if (sendOK == SOCKET_ERROR) {
        cout << "ERROR: Message did not send" << WSAGetLastError() << endl;
    }

    else {
        cout << "Message Sent Successfully" << endl;  // Confirm Client has sent message
    }

    // Confirm server received message
    char buff[1024];
    int serverSize = sizeof(server);
    int bytesR = recvfrom(sock2, buff, 1024, 0, (sockaddr*)&server, &serverSize);  // Receive confirmation message from server

    cout << "SERVER SUCCESSFULLY RECEIVED MESSAGE: " << buff << endl;
    ZeroMemory(buff, 1024);  // Zero the buffer

    // Check for errors
    if (bytesR == SOCKET_ERROR) {
        cout << "ERROR: Failure to receive confirmation that server received message" << WSAGetLastError() << endl;
        return;
    }

    
    ///// Ask for user input message
    char command;
    string s2;
    cout << "Press m to enter message, x to exit" << endl;
    cin >> command;
    cin.ignore(256, '\n');  // Will the user send a message, or exit the client program

    while (command != 'x') {  // Loop until user enters "x" to exit

        if (command == 'm' || command == 'M') {
            cout << "Enter Message: ";
            cin.ignore(0, '\n');
            //cin >> s2;
            getline(cin, s2);  // User inputs message

            int sendOK = sendto(sock2, s2.c_str(), s2.size() + 1, 0, (sockaddr*)&server, sizeof(server));  // Send user message to the server

            ///// Check for errors sending message
            if (sendOK == SOCKET_ERROR) {
                cout << "ERROR: Message did not send" << WSAGetLastError() << endl;
            }

            else {
                cout << "Message Sent Successfully" << endl;
            }

            ///// Confirm server received message
            char buff2[1024];
            int bytesR2 = recvfrom(sock2, buff2, 1024, 0, (sockaddr*)&server, &serverSize);  // Receive confirmation message from the server

            cout << "SERVER SUCCESSFULLY RECEIVED MESSAGE: " << buff2 << endl;
            ZeroMemory(buff2, 1024);  // Zero the buffer

            // Check for errors
            if (bytesR2 == SOCKET_ERROR) {
                cout << "ERROR: Failure to receive confirmation that server received message" << WSAGetLastError() << endl;
                return;
            }

        }
        cout << "Press m to enter message, x to exit" << endl;  // Ask if user wants to send another message
        cin >> command;
        cin.ignore(256, '\n');
    }
    

    // Close the socket
    closesocket(sock2);

    // Shut down winsock
    WSACleanup();

    return;
}