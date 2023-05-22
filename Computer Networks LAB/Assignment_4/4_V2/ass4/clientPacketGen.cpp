#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

using namespace std;

// Structure to hold information about single packet
struct packet
{
    short seq_num;  // Sequence Number  2 Bytes
    int time_stamp; // Time Stamp 4 Bytes
    char TTL;       // Time to Live 1 Byte
    char *payload;  // Payload P Bytes
};

int client_socket; // Used to hold the file descriptor of the client socket

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        cout << "Command Should be Like:" << endl;
        cout << argv[0] << " <ServerIP> <ServerPort> <P> <TTL> <NumPackets>" << endl;
        exit(EXIT_FAILURE);
    }

    // Getting the values of Command Line Arguments
    char *ServerIP = argv[1];
    int ServerPort = atoi(argv[2]);
    int P = atoi(argv[3]);
    if (P < 100 || P > 1000)
    {
        cout << "P is not in the Range 100 to 1000 !!" << endl;
        exit(EXIT_FAILURE);
    }
    int T = atoi(argv[4]);
    if (T % 2 != 0)
    {
        cout << "TTL is not Even !!" << endl;
        exit(EXIT_FAILURE);
    }
    if (T < 2 || T > 20)
    {
        cout << "TTL is not in the Range 2 to 20 !!" << endl;
        exit(EXIT_FAILURE);
    }
    int NumPackets = atoi(argv[5]);
    if (NumPackets < 1 || NumPackets > 50)
    {
        cout << "NumPackets is not in the Range 1 to 50 !!" << endl;
        exit(EXIT_FAILURE);
    }
    char TTL = T;

    // Creating Socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    // AF_INET for IPv4  SOCK_DGRAM for UDP  0 for Default Protocol
    if (client_socket == -1)
    {
        cout << "Failed to Create Socket !!" << endl;
        exit(EXIT_FAILURE);
    }

    /* Initializing  sockaddr_in structure to specify the address
    of the server to which the client wants to send data. */
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;                  // for IPv4
    server_address.sin_port = htons(ServerPort);          // htons for converting host byte order to network byte order
    server_address.sin_addr.s_addr = inet_addr(ServerIP); // inet_addr for converting dotted decimal to binary

    int sumRTT = 0;

    struct packet pc;                    // structure for sending packet
    struct packet recvpc;                // structure for receiving packet
    struct timeval start_time, end_time; // structure for packet sending and receiving time
    char data[1500];                     // character array for sending packet
    int RTTdata = 0;                     // variable to calculate sum of RTT values
    pc.payload = (char *)malloc(P);      // Payload of size P

    // Sending Packets
    for (short i = 0; i < NumPackets; i++) // Sending NumPackets Packets
    {
        memset(data, 0, P + 7); // Initializing data array with 0
        pc.seq_num = i;
        // start_time = {0, 0};s
        gettimeofday(&start_time, NULL);    // Getting the time of packet sending
        pc.time_stamp = start_time.tv_usec; // millisecond component
        pc.TTL = TTL;
        // Initializing Payload
        for (int j = 0; j < P; j++) // Initializing Payload with 'a'
        {
            pc.payload[j] = 'a'; // 'a' = 97
        }

        // Copying contents of structure to character array
        int bytesToBeSent = 0;                                      // Variable to keep track of number of bytes to be sent
        memcpy(data + bytesToBeSent, &(pc.seq_num), sizeof(short)); // Copying 2 bytes of seq_num
        bytesToBeSent += 2;
        memcpy(data + bytesToBeSent, &(pc.time_stamp), sizeof(int));
        bytesToBeSent += 4;
        memcpy(data + bytesToBeSent, &(pc.TTL), sizeof(char));
        bytesToBeSent += 1;
        memcpy(data + bytesToBeSent, pc.payload, P);
        bytesToBeSent += P;

        // Sending the character array to server
        int bytesSent = sendto(client_socket, data, bytesToBeSent, 0, (struct sockaddr *)&server_address, sizeof(server_address)); // 0 for default flags
        if (bytesSent == -1)                                                                                                       // Error in sending packet
        {
            cout << "Error is Sending Packet !!" << endl; // Printing the error message
            exit(EXIT_FAILURE);                           // Exiting the program
        }
        if (bytesToBeSent != bytesSent) // Error in sending packet
        {
            cout << "Sending Failed !!" << endl;                    // Printing the error message
            cout << "Bytes to be Send : " << bytesToBeSent << endl; // Printing the error message
            cout << "Bytes Sent Actually : " << bytesSent << endl;  // Printing the error message
        }

        // Receiving the packet from server
        socklen_t len2 = sizeof(server_address);                                                               // Variable to store the size of server_address
        int bytesReceived = recvfrom(client_socket, data, 1500, 0, (struct sockaddr *)&server_address, &len2); // 0 for default flags
        memcpy(&recvpc.seq_num, &data[0], sizeof(short));                                                      // Copying 2 bytes of seq_num
        memcpy(&recvpc.time_stamp, &data[2], sizeof(int));                                                     // Copying 4 bytes of time_stamp
        memcpy(&recvpc.TTL, &data[6], sizeof(char));                                                           // Copying 1 byte of TTL
        gettimeofday(&end_time, NULL);                                                                         // Getting the time of packet receiving
        RTTdata = end_time.tv_usec - recvpc.time_stamp;                                                        // Calculating RTT
        sumRTT += RTTdata;                                                                                     // Calculating sum of RTT
        cout << "Seq: " << recvpc.seq_num;                                                                     // Printing the packet details
        cout << "  RTT : " << RTTdata;                                                                         // Printing the packet details
        cout << "  TTL : " << (int)recvpc.TTL << endl;                                                         // Printing the packet details
    }
    free(pc.payload);                                            // Freeing the memory allocated to payload
    cout << endl                                                 // Printing the packet details
         << "AVG. RTT : " << (float)sumRTT / NumPackets << endl; // Printing the packet details
    return 0;                                                    // Exiting the program
}