#ifndef __SLASH_H__
#define __SLASH_H__
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>

int varFind(char * target, char** envars, int* k){//check whether the environment variable exists, return result and the variable's position in the array
  int found=0; int i = 0;
  for(; envars[i]!=NULL; i++){
    char * equ = strchr(envars[i],'=');
    *equ = '\0';
    if(!strcmp(target,envars[i])){
      found = 1;
      *equ = '=';
       break;
     }
     *equ = '=';
  }
  *k = i;
  return found;
}
void parseColon(char * pPath, std::vector<std::string> * paths){//use colon as delimiter to divide value of PATH into strings and store them in the vector
  char* mvptr = pPath;
  char* token = strsep(&mvptr,":");
  if(mvptr == NULL){//no colon found
    (*paths).push_back(token);
  }
  else{
    while(mvptr != NULL){//colon found
      (*paths).push_back(token);
      token = strsep(&mvptr,":");
    }
    (*paths).push_back(token);
  }
}
char* searchPro(std::string & path, const char * target){//use the function written in the early assignment to search for the program without '/' and return\
 the absolute path, it's a DFS search.
  DIR * dir;
  char * buf = NULL;
  struct dirent * entry;
  dir = opendir(path.c_str());
  if(dir==NULL){
        printf("here\n");
    std::cerr << "Error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
  else{
    while( (entry = readdir(dir)) != NULL ){
      if(entry->d_type & DT_DIR){
        if(entry->d_name[0]!='.'){
          char subdir[PATH_MAX+1];
          sprintf(subdir,"%s/%s",path.c_str(),entry->d_name);
          std::string sd(subdir);
          searchPro(sd,target);
        }
      }
      else{
        if(!strcmp(entry->d_name,target)){
          char rp[PATH_MAX+1];
          sprintf(rp,"%s/%s",path.c_str(),entry->d_name);
          buf = new char[PATH_MAX+1];
          realpath(rp,buf);
        }
      }
    }
  }
  if(closedir(dir)){
    std::cerr << "Error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
  return buf;
}
char* exploreDir(char ** envars, char* cmd){//iterate over each directory specified in PATH to search for the desired program
  char path[] = "PATH";
  int pos = 0;
  varFind(path,envars,&pos);
  char* pPath = strchr(envars[pos],'=')+1;
  char* temp = new char[strlen(pPath)+1];
  strcpy(temp,pPath);
  char * rpath = NULL;
  std::vector<std::string> paths;
  parseColon(temp,&paths);
  delete[] temp;
  for(std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); ++it){
    rpath = searchPro(*it,cmd);
    if(rpath) return rpath;
  }
  return NULL;
}
#endif
