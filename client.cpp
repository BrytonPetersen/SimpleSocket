#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main() {
    //save the unique ip address
    char ip[] = "127.0.0.1";
    char *serverIP = ip;
    //save the port address (you can
    //take this as a parameter as well)
    int port = 12345;
    char msg[1500];
    //set up the socket and connection tools
    struct hostent* host = gethostbyname(serverIP);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect and check the status
    int status = connect(clientSock, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if (status < 0) {
        cout<< "Error connecting to socket!" << endl;
        return 1;
        //we dont use cerr or exit() because we can run it again
    }
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    while(1) {
        cout << ">";
        string data;
        // save what they input to string data
        getline(cin, data);
        //clear the buffer
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
        if (data == "exit") {
            send(clientSock, msg, strlen(msg), 0);
            return 1;  
        }
        bytesWritten += send(clientSock, msg, strlen(msg), 0);
        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(clientSock, msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit")) {
            cout << "Server has quit the session" << endl;
            return 1;
            //this returns 1 because we dont want to quit the program quite yet. 
        }
        cout << "Server: " << msg << endl;
    }
    cout << "***********************The sesion has ended***********************" << endl;
    return 0;
}