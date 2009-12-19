// testrig for ipbatch
// just ipbatch the args
// The commands are done twice, first all at once as a vector where the 
// "commit" is implicit and then using the alternative interface of each line 
// at a time and then an explicit "commit"
// Note this second form uses a vector private to the ipbatch source file.
// Only one process using ipbatch should be active at once using this.
// Software should try to use the form with their own vector if possible. 
//

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include "ipbatch.h"

int main(int argsc, char* argsv[]) {

  int i;
  std::vector<std::string> v;

  for(i = 1; i < argsc; i++)
    v.push_back(std::string(argsv[i]));

  ipbatch(v);
#ifdef NEVER
  // and now the char * form
  for(i = 1; i < argsc; i++) {
    ipbatch(argsv[i]);
  }
  ipbatch("commit");

  std::cout << "sprintf test: " << Sprintf("ipbatch -t %s -s %d.%d.%d.%d", "mangle", 1,2,3,4) << std::endl;
#endif
  
}

  

