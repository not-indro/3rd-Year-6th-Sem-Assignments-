
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 	// inet_addr()
#include <stdlib.h>
#include <unistd.h>

using namespace std; 

int main()
{
	int server_socket, client_socket; // file descriptors
	struct sockaddr_in server_address, client_address; 		// server and client address
	socklen_t client_len = sizeof(client_address); 			// size of client address

	server_socket = socket(AF_INET, SOCK_STREAM, 0);		// create socket
	if (server_socket == -1) 							// check for error
	{
		cout << "Error creating socket." << endl;
		exit(1); 
	}

	server_address.sin_family = AF_INET; 				// set address family
	server_address.sin_port = htons(9001); 				// set port
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);	// set IP address

	bind (server_socket, (struct sockaddr *)&server_address, sizeof(server_address)); // bind socket to address
	// if (bind_status == -1) 	
	// {
		// cout << "Error binding to socket." << endl;	
		// exit(1);
	// }

	listen(server_socket, 5); // listen for connections

	client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_len); // accept connection
	if (client_socket == -1)  // check for error
	{
		cout << "Error accepting connection." << endl;  // print error message
		exit(1); // exit program
	}

	char message[256] = "Hello from server."; 			// message to send to client
	send(client_socket, message, strlen(message), 0); 	// send message to client

	close(client_socket); 								// close client socket
	close(server_socket); 								// close server socket
	return 0;
}
