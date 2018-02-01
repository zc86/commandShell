#ifndef __DEALARG_H__
#define __DEALARG_H__
void dealArg(char** argv, int total, int each){//if we encounter redirect sign in the command line, after dealing with it, we skip the sign along with the f\
ile name followed by the redirect sign
  int i = 2;
  for(; argv[total+each+i]!=NULL; i++){
    argv[total+each+i-2] = argv[total+each+i];
  }
  argv[total+each+i-2] = NULL;
}
#endif
