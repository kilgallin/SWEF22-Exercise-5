#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include<string>

static const int TRIES = 10;

/** Reads a single character from stdin
* @param allowedInput a string containing all characters that are allowed as input
* @return a character read from stdin
*/
char charInput(std::string allowedInput);
void quit(bool isLocal, bool isSrv, int currSock, int listenSock);

#endif // INPUT_H_INCLUDED
