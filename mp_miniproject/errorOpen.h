#ifndef __ERROROPEN_H__
#define __ERROROPEN_H__
void errorOpen(int fd){ //check if an error happen in the open function
  if(fd < 0){
    std::cerr << "Error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
}
#endif
