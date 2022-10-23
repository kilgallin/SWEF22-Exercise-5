#include<iostream>
#include<string>
#include <unistd.h>
#include"Input.h"
using namespace std;



char charInput(string allowedInput){
    string inputString;
    for(int i = 0; i<TRIES; i++){
        cin.clear();
        getline(cin, inputString);
        if(cin.good() && inputString.length() == 1 && allowedInput.find(inputString) != string::npos){
            return inputString.at(0);
        }
        cout<<"Wrong input, try again."<<endl;
    }
    return '\0';
}

void quit(bool isLocal, bool isSrv, int currSock, int listenSock){
    if(!isLocal){
        close(currSock);
        if(isSrv){
                close(listenSock);
        }
    }
}
