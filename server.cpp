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
    //declare constants 
    int port = 12345;
    char msg[1500];

    //set up a socket
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    /*AF_INET is an address family that is used to designate 
    the type of addresses that your socket can communicate with 
    (in this case, Internet Protocol v4 addresses). When you create
    a socket, you have to specify its address family, and then
    you can only use addresses of that type with the socket.*/
    int server1 = socket(AF_INET, SOCK_STREAM, 0);
    //SOCK_STREAM means it follows TCP. If it was SOCK_DGRAM it would be UDP
    if(server1 < 0) {
        cerr << "There was an error establishing the server socket." << endl;
        //cerr is like cout but with no buffer - it runs immediately
        exit(0);
        // return 0 would exit the function and return 0. 
        // exit(0) exits the program and returns 0.
    }
    //bind the socket to the local address
    int bindStatus = bind(server1, (struct sockaddr*) &servAddr,
        sizeof(servAddr));
    if (bindStatus < 0) {
        cerr << "Error binding socket to local address" << endl;
    }
    //once everything has passed listen for a connection (up to 5 at a time)
    cout << "Waiting for a client to connect..." << endl;
    listen(server1, 5);

    //declare the variable newSockAddr
    sockaddr_in newSockAddr;

    socklen_t newSockAddrSize = sizeof(newSockAddr);
    /*Socklen_t It is a type definition for length and size
    values used by socket related parameters, return
    values and structure members.*/    

    //accept connections
    int newSock = accept(server1, (sockaddr *)&newSockAddr, &newSockAddrSize);
    
    if (newSock < 0) {
        cerr << "Error accepting request from client." << endl;
        exit(0);
    }

    //if everything passes cout the sucessful operation
    cout << "Connected with client." << endl;
    int bytesRead, bytesWritten = 0;
    while(1) {
        //clear the buffer using memset
        memset(&msg, 0, sizeof(msg));
        //check for the keyword exit from the client
        recv(newSock, msg, sizeof(msg), 0);
        /* this is how we are receiving the message
        */
        if(!strcmp(msg, "exit")) {
            cout << "Client has left." << endl;
            //this is cout instead of cerr because we dont
            // want to quit the program
            break;
        }
        cout <<"Client: " << msg << endl;
        cout << ">";
        //declare the string 'data'
        string data;
        //getline() is equivalent to python's input()
        // it takes the cin (input) and calls it data
        getline(cin, data);
        //clear the buffer using memset
        memset(msg, 0, sizeof(msg));
        /*strcpy converts the string data into a c_string and
        then sets msg equal to it.
        C-string = char x[] = "Nice job!"
        while
        String = string x = "Nice job!"
        C-strings make it easier to iterate through the characters
        of a string - which is good if you're using memset()*/
        strcpy(msg, data.c_str());
        if(data == "exit") {
            // tell the client that the server has closed
            send(newSock, msg, strlen(msg), 0);
            break;
        }
        send(newSock, msg, strlen(msg), 0);
        //what is being sent here? why the parameters?
        // newSock = the socket being used
        // char*&msg is the string being sent
        // strlen is the length
        // 0? 
    }
    close(newSock);
    close(server1);
    cout << "***********************The sesion has ended***********************" << endl;
    return 0;
}