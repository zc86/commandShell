#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__
void initialize(char **ch, int size){//initialize a string array
  for(int i=0; i<size; i++){
    ch[i] = NULL;
  }
}
#endif
