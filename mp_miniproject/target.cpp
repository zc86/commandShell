#include <iostream>
#include <cstdlib>

int main(int argc, char ** argv, char ** envp){
  std::string line;
  while(!std::cin.eof()){
    getline(std::cin,line);
    std::cout << line << std::endl;
  }

  for(int i=1; i<argc; i++){
    if(i != argc-1){
      std::cout << argv[i] << "|";
    }
    else{
      std::cout << argv[i] << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
