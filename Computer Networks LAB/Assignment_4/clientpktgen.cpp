
/*
 * clientpktgen.cpp
 *
 * This program is a client that sends a datagram to the server.
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

#define SERVER_IP "127.0.0.1" // Localhost
#define SERVER_PORT 8080      // Port number
struct Datagram               // Datagram structure
{
    int seqNo;   // Sequence number
    int ttl;     // Time to live
    int payload; // Payload 
};
int main(int argc, char *argv[])
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (sock < 0)
    {
        cout << "Failed to create socket" << endl; // If socket creation fails
        return 1;
    }

    struct sockaddr_in server_addr;                     // Create a socket address structure
    memset(&server_addr, 0, sizeof(server_addr));       // Clear the structure
    server_addr.sin_family = AF_INET;                   // Set the address family to Internet
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Set the server IP address
    server_addr.sin_port = htons(SERVER_PORT);          // Set the server port number

const payload *time = " The RTT time = 'payload'"
int payload = atoi(argv[1]);
int = ttl = atoi(argv[2]);
int = seqNo = atoi(argv[3]);

    const char *message = "Hello, This is Indranil Bain's Server "; // Message to send
    int message_len = strlen(message);
    int bytes_sent = sendto(sock, message, message_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Send the message
    if (bytes_sent < 0)
    {
        cout << "Failed to send packet" << endl;
        return 1;
    }

    cout << "Sent " << bytes_sent << " bytes to the server" << endl;

    close(sock); // Close the socket

    return 0; // Exit
}
