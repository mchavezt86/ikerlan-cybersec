#include <iostream>
#include <cstring>
#include <sstream>
// #include <vector>
#include <fstream>
#include "functions.h"
using namespace std;

void repKeyXOR(char* input, char* output, const char* keys, uint size, uint keySize){
    // uint keyLen = sizeof(*keys);
    cout << "Key length: " << keySize << endl;
    for (uint i=0;i<size;i++){
      uint tmpKey = key[i % keySize];
      output[i] = input[i] ^ tmpKey;
    }
}