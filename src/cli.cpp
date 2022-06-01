#include <cli.h>

static char buffer[BUFFERSIZE];

static int n = sizeof(commands) / sizeof(CommandType);

void clearBuffer()  {
  // while(Serial.available()) {
  //   Serial.read();
  //   delayMicroseconds(1 / (BAUDRATE/10));
  // }
  for(int i = 0; i < BUFFERSIZE; i++) {
    buffer[i] = 0;
  }
}



// ======== DEBUGGING ========= //

void printArgs(int argc, char **args) {
  for(int i = 0; i < argc; i++) {
    char c = '.';
    int index = 0;
    Serial.print("'");
    while(c != '\0') {
      c = args[i][index++]; 
      Serial.print(c);
    }
    Serial.print("'");
    Serial.println(); 
  }
}

// ======== DEBUGGING ========= //



void readFromSerial()   {
  char c;

  static int bufferIndex = 0;

  while(Serial.available())  {
    c = Serial.read();

    if(c == '\r' || c == '\n' || bufferIndex >= BUFFERSIZE -1)  {
      int argc = 0;
      char *argv[4];

      char *token = strtok(buffer, " ");
      while(token != NULL && argc < 4)  {
        argv[argc++] = token;

        token = strtok(NULL, " ");
      }
      // printArgs(argc, argv);
      for(int i = 0; i < n; i++)  {
        if(!strcmp(commands[i].name, argv[0]))  {
          commands[i].func(argc, argv);
        }
      }
      bufferIndex = 0;
      clearBuffer();
      return;
    }
    else  {
      //sets the characters in buffer when no \r or \n
      buffer[bufferIndex] = c;
      bufferIndex++;
    }
  }
}