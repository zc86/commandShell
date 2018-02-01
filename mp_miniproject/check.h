#ifndef __CHECK_H__
#define __CHECK_H__
#include <string.h>

int checkpi(char **argv){ //check if there exists pipe in the command line
  int found = 0;
  for(int i=0; argv[i] != NULL; i++){
    if(strcmp(argv[i],"|") == 0){
      argv[i] = NULL;
      found = i;
      break;
    }
  }
  return found;
}
int checkre(char **argv){ //check if there exists any redirect in the command line
  int found = -1;
  for(int i=0; argv[i] != NULL; i++){
    if(strcmp(argv[i],"<") == 0 || strcmp(argv[i],">") == 0 || strcmp(argv[i],"2>") == 0){
      found = i;
      break;
    }
  }
  return found;
}
#endif
