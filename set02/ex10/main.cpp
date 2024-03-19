// Implement CBC mode

// CBC mode is a block cipher mode that allows us to encrypt irregularly-sized messages, despite the fact that a block cipher natively only transforms individual blocks.

// In CBC mode, each ciphertext block is added to the next plaintext block before the next call to the cipher core.

// The first plaintext block, which has no associated previous ciphertext block, is added to a "fake 0th ciphertext block" called the initialization vector, or IV.

// Implement CBC mode by hand by taking the ECB function you wrote earlier, making it encrypt instead of decrypt (verify this by decrypting whatever you encrypt to test), and using your XOR function from the previous exercise to combine them.

// The file here is intelligible (somewhat) when CBC decrypted against "YELLOW SUBMARINE" with an IV of all ASCII 0 (\x00\x00\x00 &c)

// compile using -lcryptopp -L/path/to/cryptopp/lib

#include <iostream>
#include <cstring>
#include <fstream> // File handling
// #include <iomanip> // for std::hex, std::setw, and std::setfill
// #include <vector>
#include "functions.h"
using namespace std;

// unsigned char sample [] = "Why drink the water from my hand";

int main(){
  unsigned char hexArray[4000] = {0}; // Store number as value after base64 decoding.
  unsigned char encrypted[4000] = {0};

  ifstream file("10.txt");
  string line;
  uint valIndex = 0;
  uint strLen;

  while (getline(file, line)) { //Read each line.
    const char* input = line.c_str();
    strLen = strlen(input);
    decodeBase64(input, strLen, hexArray, &valIndex);
    // memcpy(&hexArray[valIndex],input,sizeof(char)*strLen);
    // valIndex += strLen;
  }
  file.close();

  //Start with padding
  padding(hexArray,valIndex,keyLen);

//   for (uint i=0;i<100;i++){
//     cout << hexArray[i];
//   }
//   cout << endl;

//   aes128ecb_encrypt(hexArray,encrypted,32,theKey,16);

//   aes128ecb_decrypt(encrypted,decrypted,32,theKey,16);

  aes128cbc_decrypt(hexArray,encrypted,valIndex,theKey,keyLen,iv);

  for (uint i=0;i<valIndex;i++){
    cout << char(encrypted[i]);
  }
  cout << endl;

  return 0;
}