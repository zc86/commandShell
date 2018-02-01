#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "count.h"
#include "check.h"
#include "dealArg.h"
#include "errorOpen.h"

void execute_cmd(char **argv,char **envp){/* no pipe */
  pid_t cpid,w;
  int status;
  if((cpid = fork()) < 0){
    std::cerr << "Error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
  else if(cpid == 0){  //child process
    int re = checkre(argv);
    int record = 0;
    while(re >= 0){
      /* --input redirection-- */
      if(strcmp(argv[record+re],"<") == 0){
        int fd = open(argv[record+re+1],O_RDONLY);
        errorOpen(fd);
        dup2(fd,0);
        close(fd);
        dealArg(argv,record,re);
      }
      /* --output redirection-- */
      else if(strcmp(argv[record+re],">") == 0){
        int fd = open(argv[record+re+1],O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
        errorOpen(fd);
        dup2(fd,1);
        close(fd);
        dealArg(argv,record,re);
      }
      else if(strcmp(argv[record+re],"2>") == 0){
        int fd = open(argv[record+re+1],O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
        errorOpen(fd);
        dup2(fd,2);
        close(fd);
        dealArg(argv,record,re);
      }
      record = record+re;
      re = checkre(argv+record);
    }
    if(execve(argv[0],argv,envp) < 0){
      std::cerr << "Error: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else{ /*parent process*/
    do{
      w = waitpid(cpid,&status,WUNTRACED|WCONTINUED);//wait for the child to terminate
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
}
