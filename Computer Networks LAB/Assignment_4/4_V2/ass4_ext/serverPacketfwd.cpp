#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int server_socket;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Command should be like:" << endl;
        cout << argv[0] << " <ServerPort>" << endl;
    }

    // Getting the values of Command Line Arguments
    int ServerPort = atoi(argv[1]);

    // Creating Socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        cout << "Failed to Create Socket!" << endl;
        exit(EXIT_FAILURE);
    }

    // Initializing sockaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(ServerPort);
    server.sin_addr.s_addr = INADDR_ANY /* inet_addr("172.20.129.149") */
        ;

    // Binding the Socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        cout << "Failed to Bind!" << endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    char data[1307];
    while (1)
    {
        memset(data, 0, 1307);
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int bytesRecieved = recvfrom(server_socket, data, sizeof(data), 0, (struct sockaddr *)&client, &len);
        if (bytesRecieved == -1)
        {
            cout << "Error in Recieving Packets from Client !!"
                 << endl;
            close(server_socket);
            exit(EXIT_FAILURE);
        }
        data[6]--;
        int bytesSend = sendto(server_socket, data, bytesRecieved, 0, (struct sockaddr *)&client, len);
        if (bytesSend == -1)
        {
            cout << "Error in Recieving Packets from Client !!"
                 << endl;
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        if (bytesRecieved != bytesSend)
        {
            cout << "All contents are not sent properly !!" << endl;
            close(server_socket);
            exit(EXIT_FAILURE);
        }
    }
    close(server_socket);
    return 0;
}
