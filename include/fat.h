#ifndef FAT_H
#define FAT_H
#include <Arduino.h>
#include <EEPROM.h>
#define START_FAT 1
#define START_DATA 161
#define MAX_NR_FILES 10
#define MAX_FNAME_LENGHT 12

struct FatEntry{
    char fileName[MAX_FNAME_LENGHT];
    int FStartAddr;
    int FSize;
};

static FatEntry fatEntries[MAX_NR_FILES];

void initFAT();
void eraseAllFiles();
void sort();
int findFile(char *fileName);
int findFreeSpace(int fileSize);
void writeFatToEEPROM(byte nrOfFatEntry);
void store(int argc, char **args);
void retrieve(int argc, char **args);
void erase(int argc, char **args);
void files(int argc, char **args);
void freespace(int argc, char **args);
void writeData(int startaddr, int filesize, char *data);
void writeData(int startaddr, float data);
char* readData(int startaddr, int size);

#endif