#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "check.h"
#include "count.h"
#include "dealArg.h"
#include "errorOpen.h"
#include "slash.h"
#include "initialize.h"
/*--deal with redirection and pipe--*/
void execute_pipe(char **argv, char **envp){
  int pipeNum = countPipe(argv);
  int comNum = pipeNum + 1; //command number equals to the sum of pipe number and 1
  pid_t pid,w;
  int status;
  int* pipes = new int[2*pipeNum]; //need to delete it after finishing using
  int k = 0;
  for(int i=0; i< pipeNum; i++){
    if(pipe(pipes+i*2)<0){  //return the corresponding file descriptor in the int array
      std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  int t = 0;
  int pi = checkpi(argv);
  int mem = 0;
  int papro = 0;   //record if the program comes from directories in PATH, if so, need to delete the allocated space for the absolute path string
  while(comNum){
    if(!strchr(argv[mem],'/')){
      papro = 1;
      char* rpath = exploreDir(envp,argv[mem]);
      if(rpath){  //check whether the program specified without "/" exists or not
        argv[mem] = rpath;
        rpath = NULL;
      }
      else{
        std::cout << "Command " << argv[mem] << " not found" << std::endl;
        return;
      }
    }
    pid = fork();
    if( pid == -1 ){ //child progress
      std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    else if(pid == 0){
      if(pi!=0){ //not last command
        dup2(pipes[k+1],1);
      }
      if(pi == 0){//last command
        int re0 = checkre(argv+mem);
        if(re0 >= 0){
          if(strcmp(argv[mem+re0],">") == 0){
            int fd = open(argv[mem+re0+1],O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
            errorOpen(fd);
            dup2(fd,1);
            close(fd);
            dealArg(argv,mem,re0);
          }
          else if(strcmp(argv[mem+re0],"2>") == 0){
            int fd = open(argv[mem+re0+1],O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
            errorOpen(fd);
            dup2(fd,2);
            close(fd);
            dealArg(argv,mem,re0);
          }
          else{
            std::cerr << "Error: invalid format" << std::endl;
            exit(EXIT_FAILURE);
          }
        }
      }
      if(k==0){//first command
        int re = checkre(argv);
        if(re >= 0){
          if(strcmp(argv[re],"<") == 0){
            int fd = open(argv[re+1],O_RDONLY);
            errorOpen(fd);
            dup2(fd,0);
            close(fd);
            dealArg(argv,0,re);
            pi = checkpi(argv);
          }
          else{
            std::cerr << "Error: invalid format" << std::endl;
            exit(EXIT_FAILURE);
          }
        }
      }
      if(k!=0){//not first command
        dup2(pipes[k-2],0);
      }
      for(int i=0; i<2*pipeNum; i++){
        close(pipes[i]);
      }
      if(execve(argv[mem],argv+mem,envp)<0){
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    if(papro){//delete the char array allocated for the absolute path of the program
      delete[] argv[mem];
      papro = 0;
    }
    mem = pi+1+mem;
    pi = checkpi(argv+mem);
    k += 2;
    comNum--;
    t++;
  }
  for(int i=0; i<2*pipeNum; i++){
    close(pipes[i]);
  }
  delete[] pipes;
  do{//wait for the last child process to terminate
    w = waitpid(pid,&status,WUNTRACED|WCONTINUED);
    if(w == -1){
      std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    if(WIFEXITED(status)){
      std::cout << "Program exited with status " << WEXITSTATUS(status) << std::endl;
    }
    else if(WIFSIGNALED(status)){
      std::cout << "Program was killed by signal " << WTERMSIG(status) << std::endl;
    }
  }while(!WIFEXITED(status) && !WIFSIGNALED(status));
}
