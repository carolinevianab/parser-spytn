#include <stdio.h>
#include <string.h>
#include "./Parser/parser.h"

#define FALSE 0
#define TRUE 1

int main(int argc, char *argv[]) {
  char buffer[1024];
  int point = 0;
  
  int c;
  FILE *file;
    
  char filename[200];
    int isDebugOn;

 if (argc > 1) strcpy(filename, argv[1]);
 else strcpy(filename, "helloWorld.txt");
    
  if (argc > 2) isDebugOn = argv[2][0] - '0';
  else isDebugOn = FALSE;
  
  file = fopen(filename, "r");

  if (file) {
    while ((c = getc(file)) != EOF){
      buffer[point] = c;
      point++;
    }
    fclose(file);

    buffer[point] = '\0';
    printf("Arquivo \"%s\" lido.\nIniciando parser...\n\n", filename);
  
    runParser(buffer, isDebugOn);
  }
  else {
    printf("Erro! Arquivo \"%s\" não encontrado!", filename);
  }
  
  return 0;
}
