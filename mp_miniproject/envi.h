#include <string.h>
/*
  check if some arguments in string array "argv" starting with "$".
  If so, check if it exists in the environment list,
  if so, let the corresponding element in argv points to the
  value of the environment variable;
  if not, just use its original value as the argument.
*/
void dealEnv(char ** argv, char ** envars){
  for(int i=0; argv[i]!=NULL; i++){
    if(argv[i][0] == '$'){
      for(int k=0; envars[k]!=NULL; k++){
        char * equ = strchr(envars[k],'=');
        *equ = '\0';
        if(!strcmp(argv[i]+1,envars[k])){
          argv[i] = equ + 1;
          *equ = '=';
        }
        *equ = '=';
      }
    }
  }
}
/*have checked that the first argument is "set",
  then we parse this command using a different parse function*/
void parseSetEnv(char * line, char** envpairs){
  int count = 0;
  while(*line != '\0'){
    while(isspace(*line)){
      *line++ = '\0';
    }
    *envpairs++ = line;
    if(count == 2){
      return;
    }
    while(*line != '\0' && isspace(*line) == 0){
      line++;
    }
    count++;
  }
}
