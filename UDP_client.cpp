/*  Christopher Patrone
    Mika Hamiti
    Saja Freeman
    Network Design
    Phase 2
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

    [5]  Cipher Deprogres. "C++ Winsock Transfer Data with Socket". Youtube. 2018.
         URL: https://www.youtube.com/watch?v=NHrk33uCzL8
*/

#include <iostream>
#include <ws2tcpip.h>
#include <string>
//#include <sys/sendfile.h>
//#include <boost/progress.hpp>
#include <fstream> ///// For opening files

#pragma comment (lib, "ws2_32.lib")  // Add winsock library

using namespace std;


void main(int argc, char* argv[])  // Command line input
{
    // start timing
    /*progress_timer t; //if boost/progress library is used*/
    
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

    // Confirm server received message
    char buff[1024];
    int serverSize = sizeof(server);
    int bytesR = recvfrom(sock2, buff, 1024, 0, (sockaddr*)&server, &serverSize);  // Receive confirmation message from server

    cout << "SERVER SUCCESSFULLY RECEIVED MESSAGE: " << buff << endl;
    ZeroMemory(buff, 1024);  // Zero the buffer


    ///// Ask for user input message
    char command;
    string s2;
    string FileName;  ///// Name of file to send
    ofstream file;  ///// File to open
    char fBuff[1024];  ///// Buffer array for file
    int fileDownloaded = 0;  ///// Keep track of file being downloaded

    cout << "Press m to enter message, f to enter a file name, x to exit" << endl;
    cin >> command;
    cin.ignore(256, '\n');  // Will the user send a message, or exit the client program

    while (command != 'x') {  // Loop until user enters "x" to exit

        if (command == 'm' || command == 'M') {
            cout << "Enter Message: ";
            cin.ignore(0, '\n');
            //cin >> s2;
            getline(cin, s2);  // User inputs message

            int sendOK = sendto(sock2, s2.c_str(), s2.size() + 1, 0, (sockaddr*)&server, sizeof(server));  // Send user message to the server

            ///// Confirm server received message
            char buff2[1024];
            int bytesR2 = recvfrom(sock2, buff2, 1024, 0, (sockaddr*)&server, &serverSize);  // Receive confirmation message from the server

            cout << "SERVER SUCCESSFULLY RECEIVED MESSAGE: " << buff2 << endl;
            ZeroMemory(buff2, 1024);  // Zero the buffer

        }


        ///// SENDING A FILE
        ///// User inputs "f" to send a file
        else if (command == 'f') {
            cout << "Enter File Name: ";
            cin.ignore(0, '\n');
            getline(cin, FileName);  ///// User inputs name of file to send

            int sendOKf = sendto(sock2, FileName.c_str(), FileName.size() + 1, 0, (sockaddr*)&server, sizeof(server));

            ///// Check for errors sending filename
            if (sendOKf == SOCKET_ERROR) {
                cout << "ERROR: File name did not send" << WSAGetLastError() << endl;
            }

            else {
                cout << "File name Sent Successfully" << endl;
            }

            file.open(FileName, ios::binary | ios::trunc);  ///// Open file

            do {
                memset(fBuff, 0, 1024);
                int bytesR3 = recvfrom(sock2, fBuff, 1024, 0, (sockaddr*)&server, &serverSize);

                file.write(fBuff, bytesR3);
                fileDownloaded += bytesR3;
            } while (fileDownloaded < FileName.size());
            file.close();
        }


        cout << "Press m to enter message, f to enter a file name, x to exit" << endl;  // Ask if user wants to send another message
        cin >> command;
        cin.ignore(256, '\n');
    }


    // Close the socket
    closesocket(sock2);

    // Shut down winsock
    WSACleanup();

    return;
}
