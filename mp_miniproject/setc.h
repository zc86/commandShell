#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "argmax.h"
#include "initialize.h"
bool checkVar(std::string & varname){//check is the variable's name is valid
  std::string::iterator it = varname.begin();
  while(it != varname.end()){
    if( !(*it>=48 && *it<=57) && !(*it>=65 && *it<=90) && !(*it == 95) && !(*it>=97 && *it<=122) ){
      return false;
    }
    ++it;
  }
  return true;
}

int tokenNum(char** target){//count the number of tokens in the envpairs array
  int c = 0;
  while(*target!=NULL){
    c++;
    target++;
  }
  return c;
}
void handleSet(char* setLine,char **envpairs,char **envars,char **envp,int *pos,int* count){
  int exist = 0;
  parseSetEnv(setLine,envpairs);
  dealEnv(envpairs,envars);
  int tn = tokenNum(envpairs);
  if(tn < 2) return;   //if the user just type "set" in the set command then return directly
  std::string var(envpairs[1]);
  if(checkVar(var)){   //check the validity of the variable name
    for(int k=0; envars[k]!=NULL; k++){
      char *eq = strchr(envars[k],'=');
      *eq = '\0';
      if(!strcmp(envpairs[1],envars[k])){//already exists, then replace the original value with the new value
        exist = 1;
        char * rep = NULL;
        std::string temp;
        temp += envpairs[1];
        temp += "=";
        if(tn != 2) temp += envpairs[2];
        int exp = 0;
        if(!strcmp(envp[k],envars[k])){//check if this variable has been exported already
          envp[k] = NULL;
          exp = 1;
        }
        delete[] envars[k];
        rep = new char[temp.length()+1];
        strcpy(rep,temp.c_str());
        envars[k] = rep;
        if(exp) envp[k] = envars[k];
        rep = NULL;
        break;
      }
      *eq = '=';
    }
    if(!exist){//add if the key does not exist
      (*count)++;
      if(*count > ARG_MAX){
        std::cerr << "Environment list overflow" << std::endl;
        exit(EXIT_FAILURE);
      }
      std::string entr;
      entr += envpairs[1];
      entr += "=";
      if(tn != 2) entr += envpairs[2];
      char * ent = new char[entr.length()+1];
      strcpy(ent,entr.c_str());
      envars[*pos] = ent;
      ent = NULL;
      (*pos)++;
    }
  }
  else{
    std::cerr << "Invalid variable name" << std::endl;
    exit(EXIT_FAILURE);
  }
}
