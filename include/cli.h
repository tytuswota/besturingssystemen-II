#ifndef CLI_H
#define CLI_H
#define BAUDRATE 19200
#define BUFFERSIZE 32
#include "fat.h"

struct CommandType{
    char name[BUFFERSIZE];
    void (*func)(int argc,char **args);
};

//definition and init
static CommandType commands[] = {
    {"store", &store},
    {"retrieve", &retrieve},
    {"erase", &erase},
    {"files", &files},
    {"freespace", &freespace},
    // {"run", &run},
    // {"list", &list},
    // {"suspend", &suspend},
    // {"resume", &resume},
    // {"kill", &kill}
};

void clearBuffer();
void readFromSerial();

#endif