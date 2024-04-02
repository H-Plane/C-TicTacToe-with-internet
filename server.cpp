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
        if(v[i]==2 && v[i+3]==2 && v[i+6]==2)
            return 2;
        if(v[i*3]==2 && v[i*3+1]==2 && v[i*3+2]==2)
            return 2;
    }
    if(v[0]==2 && v[4]==2 && v[8]==2)
        return 2;
    return 0;
}
int main()
{

    //1. Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    //WSAStartup resturns 0 if it is successfull or non zero if failed
    if(wsaerr != 0)
    {
        cout << "The Winsock dll not found!" << endl;
        return 0;
    }
    else
    {
        cout << "The Winsock dll found" << endl;
        cout << "The status: "<< wsaData.szSystemStatus << endl;
    }

    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET; //initializing as a inivalid socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //check if creating socket is successfull or not
    if(serverSocket == INVALID_SOCKET)
    {
        cout << "Error at socket():"<< WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "socket is OK!" << endl;
    }

    //3. Bind the socket to ip address and port number
    sockaddr_in service; //initialising service as sockaddr_in structure
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(55555);
    //using the bind function
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "bind() is OK!" << endl;
    }

    //4. Listen to incomming connections
    if(listen(serverSocket, 1) == SOCKET_ERROR)
    {
        cout << "listen(): Error listening on socket: " << WSAGetLastError() << endl;
    }
    else
    {
        cout << "listen() is OK!, I'm waiting for new connections..." << endl;
    }

    //5. accepting incomming connections
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET)
    {
        cout << "accept failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    else
    {
        cout << "accept() is OK!" << endl;
    }
    while(true)
    {
        //6. (a) receiving data
        char receiveBuffer[2];
        int rbyteCount = recv(acceptSocket, receiveBuffer, 2, 0);
        if(rbyteCount < 0)
        {
            cout << "Server recv error: " << WSAGetLastError() << endl;
            return 0;
        }
        else
        {
            if(int(receiveBuffer[0])-48==9)
            {
                cout<<"Ai Castigat!"<<endl;
                return 0;
            }
            else if(int(receiveBuffer[0])==int('r'))
            {
                cout<<"Remiza"<<endl;
                return 0;
            }
            v[int(receiveBuffer[0])-48]=2;
            afis();
            if(pc())
            {
                cout<<"Ai pierdut!"<<endl;
                send(acceptSocket, "9", 2, 0);
                return 0;
            }
        }

        //6. (b)  sending data
        char buffer[2];
        printf("Where to place ""X"": ");
        cin.getline(buffer,2);
        v[int(buffer[0])-48]=1;
        int sbyteCount = send(acceptSocket, buffer, 2, 0);
        if(sbyteCount == SOCKET_ERROR)
        {
            cout << "Server send error: " << WSAGetLastError() << endl;
            return -1;
        }
    }
}
