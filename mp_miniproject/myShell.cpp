#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <string.h>
#include "parse.h"
#include "execmd.h"
#include "exepipe.h"
#include "envi.h"
#include "setc.h"
#include "slash.h"

int main(){
  int count = 0;
  std::string line;
  char buf[PATH_MAX]={'\0'};           //hold the absolute path for current working directory
  char ** argv = new char*[ARG_MAX];   //pointers to command line arguments
  char ** envp = new char*[ARG_MAX];   //pointers to environment that will be transferred to the child processes
  char ** envars = new char*[ARG_MAX]; //pointers to environment used for the parent process
  initialize(envp,ARG_MAX);
  initialize(envars,ARG_MAX);
  int pos = 0;                         //record the position that will be used for the next set command put environment var and its value in
  for(; environ[pos]!=NULL; pos++){    //initialize the envars array to contain the existing environment
    char * dup = new char[strlen(environ[pos])+1];
    strcpy(dup,environ[pos]);
    envars[pos] = dup;
    dup = NULL;
    count++;
  }
  for(int i=0; i<pos; i++){            //make the third argument of execve() function to hold the existing environment
    envp[i] = envars[i];
  }
  char *envpairs[4] = {NULL};   //hold the parsed strings in the set command
  while(1){
    if(!getcwd(buf,PATH_MAX)){  //get cwd
      std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "myShell:" << buf << " $";
    std::getline(std::cin,line);//get commandline
    if(std::cin.eof()){         //if encounter EOF then delete the allocated space and exit
      std::cout << std::endl;
      delete[] argv;
      delete[] envp;
      for(int i=0; envars[i]!=NULL; i++){
        delete[] envars[i];
      }
      delete[] envars;
      break;
    }
    if(!line.length()) continue;               //if the user hit enter directly then go to next iteration
    char * cline  = new char[line.length()+1]; //used for most comandline input
    char * setLine = new char[line.length()+1];//used for set commandline
    strcpy (setLine, line.c_str());
    strcpy(cline, line.c_str());
    parse(cline,line.length()+1,argv);         //parse the command line
    dealEnv(argv,envars);                      //deal with the environment variables at the very beginning and replace them with proper values
    std::string a0 (argv[0]);
    std::size_t fs = a0.find_first_of("/");
    if(a0.compare("exit") == 0){//exit
      delete[] argv;
      delete[] cline;
      delete[] setLine;
      delete[] envp;
      for(int i=0; envars[i]!=NULL; i++){
        delete[] envars[i];
      }
      delete[] envars;
      break;
    }
    else if(a0.compare("cd") == 0){    //change directory
      if(chdir(argv[1]) != 0){
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    else if(a0.compare("set") == 0){   //set environment variable and its value
      handleSet(setLine,envpairs,envars,envp,&pos,&count);
    }
    else if(a0.compare("export") == 0){//export the variable for other programs
      for(int i=1; argv[i]!=NULL; i++){
        int k = 0;
        int found = varFind(argv[i],envars,&k);
        if(found) envp[k] = envars[k];
        else std::cout << "Varialbe not found\n";
      }
    }
    else if(fs != std::string::npos){//execute program which has slashes in its path name
      int pi = countPipe(argv);
      if(pi) execute_pipe(argv,envp);
      else execute_cmd(argv,envp);
    }
    else{ //execute a program without slash in its path name
      int pi = countPipe(argv);
      if(pi) execute_pipe(argv,envp);
      else{
        char* rpath = exploreDir(envars,argv[0]);
        if(rpath){
          argv[0] = rpath;
          rpath = NULL;
          execute_cmd(argv,envp);
          delete[] argv[0];
        }
        else{
          std::cout << "Command " << a0 << " not found" << std::endl;
        }
      }
    }
    delete[] cline;
    delete[] setLine;
    for(int i=0;argv[i]!=NULL;i++){ //initialize argv for the next command
      argv[i] = NULL;
    }
  }
  return EXIT_SUCCESS;
}
