#include <iostream>
#include "functions.h"
using namespace std;

bool repeated(const char* s1, const char* s2, uint total){

    for (uint i=0;i<total;i++){
      if ((s1[i] ^ s2[i]) != 0){
        return false;
      }
    }
    return true;
}