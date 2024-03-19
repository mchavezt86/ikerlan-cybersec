// Detect AES in ECB mode

// In this file are a bunch of hex-encoded ciphertexts.

// One of them has been encrypted with ECB.

// Detect it.

// Remember that the problem with ECB is that it is stateless and deterministic; the same 16 byte plaintext block will always produce the same 16 byte ciphertext.

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include "functions.h"
using namespace std;

int main(){
  unsigned char hexArray[4000] = {0}; // Store number as value after base64 decoding.

  ifstream file("8.txt");
  string line;
  uint valIndex = 0;
  uint strLen;
  uint keyLen = 16;
  bool ECB = false;

  while (getline(file, line)) { //Read each line.
    const char* input = line.c_str();
    strLen = strlen(input);
    
    for (uint i=0;i<strLen-keyLen;i+=keyLen){
      for (uint j=i+keyLen;j<strLen;j+=keyLen){
        if ( repeated(&(input[i]),&(input[j]),keyLen) ){
          cout << input << endl;
          ECB = true;
          break;
        }
        // for (uint k=0;k<keyLen;k++){
        //   cout << input[i+k];
        // }
        // cout << " ,";
        // for (uint k=0;k<keyLen;k++){
        //   cout << input[j+k];
        // }
        // cout << endl;
      }
      if (ECB){
        break;
      }
    }
  }
  file.close();
  
  return 0;
}