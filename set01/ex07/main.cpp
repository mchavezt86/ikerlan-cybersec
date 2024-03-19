// AES in ECB mode

// The Base64-encoded content in this file has been encrypted via AES-128 in ECB mode under the key

// "YELLOW SUBMARINE".

// (case-sensitive, without the quotes; exactly 16 characters; I like "YELLOW SUBMARINE" because it's exactly 16 bytes long, and now you do too).

// Decrypt it. You know the key, after all.

// Easiest way: use OpenSSL::Cipher and give it AES-128-ECB as the cipher. 

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include <iomanip> // for std::hex, std::setw, and std::setfill
#include <vector>
#include "functions.h"
using namespace std;

int main(){
  unsigned char hexArray[4000] = {0}; // Store number as value after base64 decoding.
//   char output[4000] = {0};

  ifstream file("7.txt");
  string line;
  uint valIndex = 0;
  uint strLen;

  while (getline(file, line)) { //Read each line.
    const char* input = line.c_str();
    strLen = strlen(input);
    decodeBase64(input, strLen, hexArray, &valIndex);
  }
  file.close();

  unsigned char output[valIndex] = {0};

  aes128ecb(hexArray, output, valIndex, theKey, keyLen);

  for(uint i=0;i<valIndex;i++){
    // cout << output[i];
    // cout << hex << setw(2) << setfill('0') << uint(output[i]);
    // cout << hex << setw(2) << setfill('0') << uint(hexArray[i]) << ", ";
  }
  cout << endl;

  return 0;
}