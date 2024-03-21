#include <iostream>
#include "functions.h"
using namespace std;

/* function repeated: Finds if two char arrays have the same data.
   - input: two char * to the inputs, uint for their size
   - output: boolean, true if the are repeated
*/
bool repeated(const char* s1, const char* s2, uint total){

    for (uint i=0;i<total;i++){
      // XOR against same data produces a 0
      if ((s1[i] ^ s2[i]) != 0){
        // if there is one difference just return false.
        return false;
      }
    }
    return true;
}