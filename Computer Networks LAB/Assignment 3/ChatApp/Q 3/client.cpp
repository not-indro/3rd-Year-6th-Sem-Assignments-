
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    int client_socket; // file descriptors
    struct sockaddr_in server_address; // server and client address

    client_socket = socket(AF_INET, SOCK_STREAM, 0);     // create socket
    if (client_socket == -1)                           // check for error
    {
        cout << "Error creating socket." << endl; 
        exit(1);
    }

    server_address.sin_family = AF_INET; // set address family
    server_address.sin_port = htons(9001); // set port
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // set IP address

    int connect_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)); // connect to server
    if (connect_status == -1)
    {
        cout << "Error connecting to server." << endl;  // print error message
        exit(1); // exit program
    }

    char server_response[256]; // message to send to client
    recv(client_socket, &server_response, sizeof(server_response), 0); // receive message from server
    cout << "Server response: " << server_response << endl; 

    close(client_socket); // close socket

    return 0; 
}
