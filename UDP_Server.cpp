/*  Christopher Patrone
    Mika Hamiti
    Saja Freeman
    Network Design
    Phase 2
    This program is written in C++, and the goal is to create a UDP server and client that
    can send messages back and forth.  Phase 2 aims to send files from the client to the server.

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

    [5]  Cipher Deprogres. "C++ Winsock Transfer Data with Socket". Youtube. 2018.
         URL: https://www.youtube.com/watch?v=NHrk33uCzL8

*/

#include <iostream>
#include <ws2tcpip.h>
#include <fstream>  ///// For opening files
#include "atlstr.h"  ///// For opening bitmap images
#include <windows.h>
#include <string>
//#include <sys/sendfile.h>

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

        // Check for errors
        if (bytesRec == SOCKET_ERROR) {
            cout << "ERROR: Failure to receive from client" << WSAGetLastError() << endl;
            return;
        }

        ////////////////////////////////////////////////////////////////////
        ifstream in;  // File to copy from
        in.open("Pier.bmp", ios::binary);
        /* use the function to recv file name here */
        ofstream ofile;
        ofile.open(buffer, ios::binary);

        if (ofile.is_open())
            cout << "File " << buffer << " is open" << endl;
        else {
            cout << "File could not be opened" << endl;
            return;
        }

        // receive file data
        /*int Rem_data = 0;
        while (Rem_data > 1024)
        {
            int bytes = recvfrom(sock, buffer, 1024, 0, (sockaddr*)&client, &clientLength);
            ofile.put(in.get());;
            Rem_data -= bytes;
        }*/

        for (int i = 0; i < 1920 * 1080 * 3 + 54; i++)  // Draw the bmp image to the output file
            ofile.put(in.get());


        ofile.close();
        in.close();
        ////////////////////////////////////////////////////////////////

        // Client message information
        char clientIP[256];
        ZeroMemory(clientIP, 256);  // Zero buffer for client message

        // Converts from internet address in binary format to standard text format
        inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

        // Output message received from client and the client's IP address
        cout << "Message received from " << clientIP << " : " << buffer << endl;


        /// Send message back to client, confirming message has been received
        int sendOkay = sendto(sock, buffer, 1024, 0, (sockaddr*)&client, clientLength);

        /// Check for errors sending message
        if (sendOkay == SOCKET_ERROR) {
            cout << "ERROR: Message did not send back to client" << WSAGetLastError() << endl;
        }

        ///// Try to open a file
        /*ifstream file;
        file.open(buffer, ios::binary);

        if (file.is_open()) {
            cout << "File " << buffer << " is open" << endl;
            file.seekg(0, ios::beg);

            // read file with do-while loop
            do {
                // read and send part file to client
                file.read(buffer, 1024);


                /////  Load Bitmap Image
                if (file.gcount() > 0) {
                    int SendInfo = sendto(sock, buffer, file.gcount(), 0, (sockaddr*)&client, clientLength);

                    if (SendInfo == SOCKET_ERROR) {
                        // error sending data - break loop
                        cout << "ERROR: Data was not sent" << endl;
                        return;
                    }
                }
            } while (file.gcount() > 0);
            file.close();
        }
        */
    }

    // Close socket
    closesocket(sock);

    // Shut down winsock
    WSACleanup();

    return;
}
