#include <iostream>
#include <cstring>
#include <sstream>
// #include <vector>
#include <fstream>
#include "functions.h"
using namespace std;

/* function repKeyXOR:
   - input: char * to the input, char * to the output, uint for array size, and uint size of the key. The actual
    key is in the header file.
   - output: stored on the char * output.
*/
void repKeyXOR(char* input, char* output, const char* keys, uint size, uint keySize){
    
    cout << "Key length: " << keySize << endl;
    
    /* Main loop: iterate over the whole input array, while rotating the array that contains the key for XORing*/
    for (uint i=0;i<size;i++){
      uint tmpKey = key[i % keySize];
      output[i] = input[i] ^ tmpKey;
    }
}