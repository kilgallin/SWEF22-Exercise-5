#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

int prepareSocket(const char * listenAddr, const char * port, bool isServer);
int establishConn(int& cliSock, int& listenSock, int& clientSock, bool& isSrv, bool& isCli);

#endif // SERVER_H_INCLUDED
