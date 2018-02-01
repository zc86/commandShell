#ifndef __ARGMAX_H__
#define __ARGMAX_H__
const int ARG_MAX = 1024; //the initial size of argv, envars and envp
#endif
// I know it is bad to assume the limit of the number of arguments.
// I tried to double the size every time the corresponding array has been used up (amortized method) but failed :(
