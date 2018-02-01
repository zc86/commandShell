#ifndef __COUNT_H__
#define __COUNT_H__

int countPipe(char ** argv){ //count the number of pipes signs
  int ct = 0;
  for(int i=0; argv[i]!=NULL; i++){
    if(!strcmp(argv[i],"|")) ct++;
  }
  return ct;
}
int countRedirect(char * line){ //count the number of redirect signs
  int ct = 0;
  for(int i=0; line[i]!='\0'; i++){
    if(line[i] == '>' || line[i] == '<'){
      ct++;
    }
  }
  return ct;
}
#endif
