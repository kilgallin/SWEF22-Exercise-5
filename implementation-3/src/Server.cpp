#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <netdb.h>
#include"Input.h"
using namespace std;

static const string DEFAULT_PORT = "8093", DEFAULT_ADDRESS = "localhost";

int prepareSocket(const char * listenAddr, const char * port, bool isServer){

    struct addrinfo hints, *serv_info;
    int opt=1, fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(listenAddr, port, &hints, &serv_info) != 0){
        return -1;
    }

    fd = socket(serv_info->ai_family, serv_info->ai_socktype, 0);
    if(fd == -1){
        freeaddrinfo(serv_info);
        return -1;
    }

    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt) != 0){
        freeaddrinfo(serv_info);
        return -1;
    }

    if(isServer){
        if(bind(fd,serv_info->ai_addr, serv_info->ai_addrlen) != 0){
            freeaddrinfo(serv_info);
            return -1;
        }
        freeaddrinfo(serv_info);
        if(listen(fd, 10) != 0){
            freeaddrinfo(serv_info);
            return -1;
        }
    } else {
        if(connect(fd, serv_info->ai_addr, serv_info->ai_addrlen) != 0){
            cout<<"error\n";
            freeaddrinfo(serv_info);
            return -1;
        }
        freeaddrinfo(serv_info);
    }


    return fd;
}

int establishConn(int& cliSock, int& listenServSock, int& connectionServSock, bool& isSrv, bool& isCli){
    string address, port;
    cout<<"Run as server or client? (s/c)"<<endl;
    char tmpChar = charInput("sc");
    if(tmpChar == '\0'){
        return 1;
    }
    cout<<"Enter address (press enter to use "<<DEFAULT_ADDRESS<<")"<<endl;
    do{
        cin.clear();
        getline(cin, address);
    } while (!cin.good());
    if(address.size() == 0){
        address = DEFAULT_ADDRESS;
    }
    cout<<"Enter port number (press enter to use the default port)"<<endl;
    do{
        cin.clear();
        getline(cin, port);
    } while (!cin.good());
    if(port.size() == 0){
        port = DEFAULT_PORT;
    }
    if(tmpChar == 'c'){
        isCli = true;
        cliSock = prepareSocket(&address[0], &port[0], false);
        if (cliSock == -1){
            return 1;
        }
    }
    else {
        isSrv = true;
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof their_addr;
        listenServSock = prepareSocket(&address[0], &port[0], true);
        if (listenServSock == -1){
            return 1;
        }
        cout<<"Waiting for your opponent..."<<endl;
        connectionServSock = accept(listenServSock, (struct sockaddr*)&their_addr, &addr_size);
    }
    return 0;
}
