// Detect AES in ECB mode

// In this file are a bunch of hex-encoded ciphertexts.

// One of them has been encrypted with ECB.

// Detect it.

// Remember that the problem with ECB is that it is stateless and deterministic; the same 16 byte plaintext block will always produce the same 16 byte ciphertext.

/* Simple logic: try to find data (with size of the block) that is repeated in the string*/

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
    
    /* This two for-loops handles the main iteration the idea behind this is:
      - Get first block and compares with the second block, then the third... until the last
        block is reached.
      - The process repetas now for the second block: it is compared to the third block, then
        the fourth ... until the last block.
      - The iteration continues until the second last block */
    for (uint i=0;i<strLen-keyLen;i+=keyLen){
      for (uint j=i+keyLen;j<strLen;j+=keyLen){
        if ( repeated(&(input[i]),&(input[j]),keyLen) ){
          cout << input << endl;
          ECB = true;
          break;
        }
      }
      if (ECB){
        break;
      }
    }
  }
  file.close();
  
  return 0;
}