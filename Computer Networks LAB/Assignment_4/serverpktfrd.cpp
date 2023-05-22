
/*
 * serverpktfrd.cpp
 * This program is a server that receives a datagram from the client.
 * It then forwards the datagram to another server.
 * Name - Indranil bain
 * ENo. - 2020csb039
 * Subject - Computer Networks Lab
 */

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

using namespace std;

#define SERVER_PORT 8080 // Port number

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (sock < 0)                              // If socket creation fails
    {
        cout << "Failed to create socket" << endl; // If socket creation fails
        return 1;
    }

    struct sockaddr_in server_addr;               // Create a socket address structure
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure
    server_addr.sin_family = AF_INET;             // Set the address family to Internet
    server_addr.sin_addr.s_addr = INADDR_ANY;     // Set the server IP address
    server_addr.sin_port = htons(SERVER_PORT);    // Set the server port number

    int bind_result = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Bind the socket to the server address
    if (bind_result < 0)                                                                // If binding fails
    {
        cout << "Failed to bind socket" << endl;
        return 1;
    }

    cout << "Waiting for packets..." << endl; // Wait for packets

    char buffer[1024]; // Buffer to hold the received packet
    while (true)       // Loop forever
    {
        struct sockaddr_in client_addr;                                                                                    // Create a socket address structure
        socklen_t client_addr_len = sizeof(client_addr);                                                                   // Set the size of the structure
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len); // Receive the packet
        if (bytes_received < 0)                                                                                            // If receiving fails
        {
            cout << "Failed to receive packet" << endl;
            break;
        }

        cout << "Received " << bytes_received << " bytes from client " << inet_ntoa(client_addr.sin_addr) << endl;

        cout << "Packet contents: " << buffer << endl; // Print the packet contents
    }

    close(sock); // Close the socket

    return 0; // Exit the program
}
