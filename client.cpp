#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
using namespace std;
int v[9]= {0};
void afis()
{
    for(int i=0; i<9; i++)
    {
        if(v[i]==0)
            cout<<i;
        else if(v[i]==1)
            cout<<"X";
        else
            cout<<"O";
        if(!((i+1)%3))
        {
            cout<<endl;
            if(i!=8)
                cout<<"-+-+-"<<endl;
        }
        else
            cout<<"|";
    }
}
int pc()
{
    for(int i=0; i<3; i++)
    {
        if(v[i*3]==1 && v[i*3+1]==1 && v[i*3+2]==1)
            return 1;
        if(v[i]==1 && v[i+3]==1 && v[i+6]==1)
            return 1;
    }
    if(v[0]==1 && v[4]==1 && v[8]==1)
        return 1;
    return 0;
}
int main()
{
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if(wserr != 0)
    {
        cout << "The winsock dll not found" << endl;
        return 0;
    }
    else
    {
        cout << "The Winsock dll found" << endl;
        cout << "The status: "<< wsaData.szSystemStatus << endl;
    }

    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET)
    {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "socket is OK!" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(55555);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "Client: Connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }
    int cnt=0;
    afis();
    while(true)
    {
        char buffer[2];
        printf("Where to place ""O"": ");
        cin.getline(buffer,2);
        v[int(buffer[0])-48]=2;
        int sbyteCount = send(clientSocket, buffer, 2, 0);
        if(sbyteCount == SOCKET_ERROR)
        {
            cout << "Server send error: " << WSAGetLastError() << endl;
            return -1;
        }
        char receiveBuffer[2];
        int rbyteCount = recv(clientSocket, receiveBuffer, 2, 0);
        if(rbyteCount < 0)
        {
            cout << "Client recv error: " << WSAGetLastError() << endl;
            return 0;
        }
        else
        {
            cnt++;
            if(int(receiveBuffer[0])-48==9)
            {
                cout<<"Ai Castigat!"<<endl;
                return 0;
            }
            v[int(receiveBuffer[0])-48]=1;
            afis();
            if(cnt==4)
            {
                cout<<"Remiza"<<endl;
                send(clientSocket, "r", 2, 0);
                return 0;
            }
            if(pc())
            {
                cout<<"Ai pierdut!"<<endl;
                send(clientSocket, "9", 2, 0);
                return 0;
            }
        }
    }
    return 0;
}
