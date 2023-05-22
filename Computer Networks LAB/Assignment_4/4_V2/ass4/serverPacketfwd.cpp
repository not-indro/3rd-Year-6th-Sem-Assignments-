#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int server_socket;
int main(int argc, char *argv[]) // Command Line Arguments
{
    if (argc != 2) // Checking for the number of arguments
    {
        cout << "Command should be like:" << endl;  // Printing the error message
        cout << argv[0] << " <ServerPort>" << endl; // Printing the error message
    }

    // Getting the values of Command Line Arguments
    int ServerPort = atoi(argv[1]);

    // Creating Socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET for IPv4  SOCK_DGRAM for UDP  0 for Default Protocol
    if (server_socket == -1)                        // Checking for error
    {
        cout << "Failed to Create Socket!" << endl; // Printing the error message
        exit(EXIT_FAILURE);                         // Exiting the program
    }

    // Initializing sockaddr_in structure
    struct sockaddr_in server;           // Server Address
    server.sin_family = AF_INET;         // IPv4
    server.sin_port = htons(ServerPort); // Port Number
    server.sin_addr.s_addr = INADDR_ANY  /* inet_addr("172.20.129.149") */
        ;

    // Binding the Socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == -1) // Checking for error
    {
        cout << "Failed to Bind!" << endl; // Printing the error message
        close(server_socket);              // Closing the socket
        exit(EXIT_FAILURE);                // Exiting the program
    }

    char data[1307]; // Data Buffer
    while (1)        // Infinite Loop
    {
        memset(data, 0, 1307);                                                                                // Clearing the data buffer
        struct sockaddr_in client;                                                                            // Client Address
        socklen_t len = sizeof(client);                                                                       // Length of Client Address
        int bytesReceived = recvfrom(server_socket, data, sizeof(data), 0, (struct sockaddr *)&client, &len); // Receiving the packet from the client
        cout << bytesReceived << endl;                                                                        // Printing the number of bytes received
        if (bytesReceived == -1)                                                                              // Checking for error
        {
            cout << "Error in Receiving Packets from Client !!" // Printing the error message
                 << endl;
            close(server_socket); // Closing the socket
            exit(EXIT_FAILURE);   // Exiting the program
        }
        data[6]--;                                                                                      // Decreasing the TTL by 1
        int bytesSend = sendto(server_socket, data, bytesReceived, 0, (struct sockaddr *)&client, len); // Sending the packet to the client
        if (bytesSend == -1)                                                                            // Checking for error
        {
            cout << "Error in Receiving Packets from Client !!" // Printing the error message
                 << endl;
            close(server_socket); // Closing the socket
            exit(EXIT_FAILURE);   // Exiting the program
        }

        if (bytesReceived != bytesSend) // Checking if all the contents are sent properly
        {
            cout << "All contents are not sent properly !!" << endl; // Printing the error message
            close(server_socket);                                    // Closing the socket
            exit(EXIT_FAILURE);                                      // Exiting the program
        }
    }
    close(server_socket); // Closing the socket
    return 0;
}