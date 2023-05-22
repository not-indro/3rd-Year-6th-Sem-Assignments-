#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_addr()
#include <unistd.h> 
#include <string.h> 
using namespace std;

int main()
{

    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);                                // Create a socket
    struct sockaddr_in server_address;                                                  // Create a socket address
    server_address.sin_family = AF_INET;                                                //  Set the address family to Internet
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");                            // Set the server IP address
    server_address.sin_port = htons(8080);                                              // Set the server port
    connect(client_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)); // Connect to the server
    bool flag = true;                                                                   // To check if the client wants to send more messages
    int cnt = 0;                                                                        // To check if the client wants to send more messages
    while (flag)
    {                                                      
        char text[500];                                // To store the message to be sent to the server
        cout << "Type your text and send to the server: "; // Prompt the user to enter a message

        if (cnt != 0)                // To ignore the first cin.ignore() call
            cin.ignore(60000, '\n'); // To ignore the newline character
        cnt++;
        cin.getline(text, 1000); 

        send(client_sockfd, text, sizeof(text), 0); // Send the message to the server

        char buffer[1000];
        int bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0); // Receive the message from the server
        buffer[bytes_received] = '\0';
        if (bytes_received < 0)
        {
            cout << " ERROR! " << endl; // If the server is not running
            break;
        }

        int k;                                              // To check if the client wants to send more messages
        cout << "Text received: " << buffer << std::endl; // Print the message received from the server
        cout << "Enter 2 for more messages: ";           // Prompt the user to enter 5 to send more messages
        cin >> k;
        if (k != 2) // If the user enters a number other than 2, the client will not send more messages
            flag = false;
    }

    close(client_sockfd); // Close the socket

    return 0; // Exit the program
}
