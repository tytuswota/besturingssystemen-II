#include <fat.h>

EERef nrOfFiles = EEPROM[0];

void initFAT() {
  eraseAllFiles();
  Serial.println("FAT initialized");
  if(nrOfFiles > 10) nrOfFiles = 10;
}

void eraseAllFiles() {
  nrOfFiles = 0;
}

void sort() {
  for (int i = 0; i < nrOfFiles - 1; i++) {
    if (fatEntries[i].FStartAddr == 0) {
      for(int j = i; j < nrOfFiles - 1; j++)  {
        fatEntries[j].FSize = fatEntries[j + 1].FSize;
        fatEntries[j].FStartAddr = fatEntries[j + 1].FStartAddr;
        strcpy(fatEntries[j].fileName, fatEntries[j + 1].fileName);
        writeFatToEEPROM(j);
      }
      break;
    }
  }
}

int findFile(char *fileName)   {
  for (int i = 0; i < nrOfFiles; i++) {
    if (!strcmp(fatEntries[i].fileName, fileName)) return i;
  }
  return -1;
}

int findFreeSpace(int fileSize) {
  int prev = START_DATA;
  if(nrOfFiles == 0) return START_DATA;
  for(int i = 0; i < nrOfFiles; i++) {
    if(fatEntries[i].FStartAddr - prev > fileSize) return prev + 1;
    else prev = fatEntries[i].FStartAddr + fatEntries[i].FSize;
  }
  if(EEPROM.length() - prev > fileSize) return prev;
  return -1;
}

void writeFatToEEPROM(byte nrOfFatEntry) {
  byte startOfFatAddress = sizeof(nrOfFiles) + nrOfFatEntry * sizeof(FatEntry);
  EEPROM.put(startOfFatAddress, fatEntries[nrOfFatEntry]);
}

void store(int argc, char **args)   {
  Serial.println(F("in_store"));
  char *filename = args[1];
  int filesize = atoi(args[2]);
  
  char *content = args[3];

  Serial.println(F("===========args"));
  Serial.println(content);
  Serial.println(F("===========args"));
  // if(strcmp(args[3], "") != 0) content = args[3];
  if(nrOfFiles >= MAX_NR_FILES) {
    Serial.println(F("Nr of files exceeds maximum"));
    return;
  }
  int f = findFreeSpace(filesize);
  if (f == -1) {
    Serial.println(F("Not enough space"));
    return;
  }
  if (findFile(args[1]) != -1) {
    Serial.println(F("File already exists"));
    return;
  }

  strcpy(fatEntries[nrOfFiles].fileName, filename);
  fatEntries[nrOfFiles].FSize = filesize;
  fatEntries[nrOfFiles].FStartAddr = f; 
  writeFatToEEPROM(nrOfFiles);
  nrOfFiles++;
  writeData(f, filesize, content);
}

void retrieve(int argc, char **args)    {
  Serial.print(F("Retrieving file: "));
  Serial.println(args[1]);
  int index = findFile(args[1]);
  if (index == -1) {
      Serial.println(F("File not found"));
      return;
  }
  Serial.print(F("Filename: "));
  Serial.print(fatEntries[index].fileName);
  Serial.print(F(" Filesize: "));
  Serial.print(fatEntries[index].FSize);
  Serial.print(F(" start address: "));
  Serial.println(fatEntries[index].FStartAddr);
  char *s = readData(fatEntries[index].FStartAddr, fatEntries[index].FSize);
  Serial.println(s);
}

void erase(int argc, char **args) {
  int f = findFile(args[1]);
  if (f == -1) {
      Serial.println(F("File not found."));
      return;
  }
  strcpy(fatEntries[f].fileName, "");
  fatEntries[f].FStartAddr = 0;
  fatEntries[f].FSize = 0;
  writeFatToEEPROM(f);
  sort();
  nrOfFiles--;
  Serial.print(F("File '"));
  Serial.print(args[1]);
  Serial.println(F("' erased"));
}

void files(int argc, char **args)   {
  FatEntry file;
  for(int i = 0; i < nrOfFiles; i++)   {
    
    int addres = 2;

    if(i > 0){
      addres = ((i*sizeof(file)) + 2);
    }

    EEPROM.get(addres, file);
    
    Serial.print(i);
    Serial.print(F(" Filename: '"));
    Serial.print(file.fileName);
    Serial.print(F("' Filesize: "));
    Serial.print(file.FSize);
    Serial.print(F(" start address: "));
    Serial.println(file.FStartAddr);
  }
}

void freespace(int argc, char **args)   {
  int length = EEPROM.length() - 161;
  for(int i = 0; i< nrOfFiles;i++)  {
    length -= fatEntries[i].FSize;
  }
  Serial.println(length);
}

void writeData(int startaddr, int filesize, char *data)  {
  for(int i = 0; i < filesize; i++) {
    EEPROM.write(startaddr+i, data[i]);
  }
}

void writeData(int startaddr, float data)  {
  EEPROM.put(startaddr, data);
}

char* readData(int startaddr, int size)  {
  char s[size];
  for(int i = 0; i < size; i++) {
    s[i] = (char)EEPROM.read(startaddr + i);
    Serial.print(s[i]);
  }
  return s;
}

void showMem(int argc,char **args)  {
  for(int i = 0; i <EEPROM.length(); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(EEPROM.read(i));
  }
}