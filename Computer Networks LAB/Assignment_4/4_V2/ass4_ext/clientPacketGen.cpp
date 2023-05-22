#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>

using namespace std;

struct packet
{
    short seq_num;
    int time_stamp;
    char TTL;
    char *payload;
};

int client_socket;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Command Should be Like:" << endl;
        cout << argv[0] << " <ServerIP> <ServerPort> <TTL> <Filename>" << endl;
        exit(EXIT_FAILURE);
    }

    char *ServerIP = argv[1];
    int ServerPort = atoi(argv[2]);

    int T = atoi(argv[3]);
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
    // char fileName[] = "./data.txt";
    cout << argv[4];
    // strcat(fileName, argv[4]);
    char TTL = T;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        cout << "Failed to Create Socket !!" << endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(ServerPort);
    server_address.sin_addr.s_addr = inet_addr(ServerIP);

    struct packet pc;
    struct packet recvpc;
    struct timeval start_time, end_time;
    char data[1500];
    int RTTdata = 0;

    int fd = open(argv[4], O_WRONLY | O_CREAT, 0777);
    for (int P = 100; P <= 1000; P += 100)
    {
        int sumRTT = 0;
        pc.payload = (char *)malloc(P);

        for (short i = 0; i < 49; i++)
        {
            memset(data, 0, P + 7);
            pc.seq_num = i;
            start_time = {0, 0};
            gettimeofday(&start_time, NULL);
            pc.time_stamp = start_time.tv_usec;
            pc.TTL = TTL;
            for (int j = 0; j < P; j++)
            {
                pc.payload[j] = 'a';
            }
            recvpc.TTL = TTL;

            int bytesToBeSent = 0;
            memcpy(data + bytesToBeSent, &(pc.seq_num), sizeof(short));
            bytesToBeSent += 2;
            memcpy(data + bytesToBeSent, &(pc.time_stamp), sizeof(int));
            bytesToBeSent += 4;
            memcpy(data + bytesToBeSent, &(pc.TTL), sizeof(char));
            bytesToBeSent += 1;
            memcpy(data + bytesToBeSent, pc.payload, P);
            bytesToBeSent += P;

            while (pc.TTL > 1)
            {
                int bytesSent = sendto(client_socket, data, bytesToBeSent, 0, (struct sockaddr *)&server_address, sizeof(server_address));
                if (bytesSent == -1)
                {
                    cout << "Error is Sending Packet !!" << endl;
                    exit(EXIT_FAILURE);
                }
                if (bytesToBeSent != bytesSent)
                {
                    cout << "Sending Failed !!" << endl;
                    cout << "Bytes to be Send : " << bytesToBeSent << endl;
                    cout << "Bytes Sent Actually : " << bytesSent << endl;
                }

                socklen_t len2 = sizeof(server_address);
                int bytesReceived = recvfrom(client_socket, data, 1500, 0, (struct sockaddr *)&server_address, &len2);

                memcpy(&pc.seq_num, &data[0], sizeof(short));
                memcpy(&pc.time_stamp, &data[2], sizeof(int));
                memcpy(&pc.TTL, &data[6], sizeof(char));
                data[6]--;
            }
            gettimeofday(&end_time, NULL);
            RTTdata = end_time.tv_usec - pc.time_stamp;
            char data[20];
            sprintf(data, "%d %d\n", P, RTTdata);
            write(fd, data, strlen(data));
            sumRTT += RTTdata;
        }

        free(pc.payload);
    }

    return 0;
}