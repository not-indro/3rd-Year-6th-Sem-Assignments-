#include <iostream>
#include <netinet/in.h> 
#include <unistd.h>
#include <string.h>
using namespace std;

int main()
{

	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
  
	struct sockaddr_in server_address;   // Create a socket address
	server_address.sin_family = AF_INET;   //  Set the address family to Internet
	server_address.sin_addr.s_addr = INADDR_ANY; // Set the server IP address
	server_address.sin_port = htons(8080); // Set the server port

	bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)); // Bind the socket to the address

	listen(server_sockfd, 5); // Listen for connections

	int client_sockfd = accept(server_sockfd, nullptr, nullptr); // Accept a connection

	while (true)
	{
		char message[1000];

		int bytes_received = recv(client_sockfd, message, sizeof(message), 0); // Receive the message from the client
		if (bytes_received < 0) // If the client is not running
		{
			cout << " ERROR! " << endl;
			break;
		}
		if (bytes_received == 0)
			break;
		cout << " Text received: " << message << endl;

		send(client_sockfd, message, bytes_received, 0); // Send the message to the client
		bytes_received = -1;
	}
	close(client_sockfd); // Close the socket
	return 0;
}
