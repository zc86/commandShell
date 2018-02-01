#include <stdlib.h>
#include "argmax.h"
#include <iostream>
/* Method:
1) replace the escaped space by percent symbol;
2) split the command line by normal space;
3) finally replace percent back to space.
*/
void parseEscSpace(char * line, char * backup, int* mark){//parse the escaped space -- create a new array and replace "\ " by '%'
  int i = 0;
  while(*line != '\0'){
    if(*line == '\\'){
      ++line;
      if(*line == ' '){
        backup[i] = '%';
        *mark++ = i;  //record the positon "\ " replaced by "%"
        ++line;
      }
      else{
        backup[i] = '\\';
      }
    }
    else{
      backup[i] = *line;
      ++line;
    }
    i++;
  }
}
void parse(char* line, size_t len, char **cargv){
  int counter = 0;
  int* mark = new int[len];//it is used to record where the escaped spaces are replaced by the percent symbol
  for(size_t i=0; i<len; i++) mark[i] = -1;//initialization
  char* backup = new char[len];
  for(size_t i=0;i<len;i++) backup[i] = '\0';//initialization
  parseEscSpace(line,backup,mark);
  char ** temp = cargv;
  for(size_t i=0; i<len; i++){
    line[i]='\0';
  }
  for(size_t i=0; i<len; i++){
    line[i] = backup[i];
  }
  char* mline = line;
  while(*line != '\0'){
    while(isspace(*line)){
       *line++ = '\0';
    }
    if(*line != '\0'){
      counter++;
      if(counter > ARG_MAX-1){
        std::cerr << "Arguments overflow" << std::endl;
        exit(EXIT_FAILURE);
      }
      *temp++ = line;
    }
    while (*line != '\0' && isspace(*line) == 0){
       line++;
    }
  }
  *temp = NULL;
  for(int i=0; mark[i] != -1; i++){//replace the corresponding "%" to " "
    mline[mark[i]] = ' ';
  }
  delete[] mark;
  delete[] backup;
}
