// Implement repeating-key XOR

// Here is the opening stanza of an important work of the English language:

// Burning 'em, if you ain't quick and nimble
// I go crazy when I hear a cymbal

// Encrypt it, under the key "ICE", using repeating-key XOR.

// In repeating-key XOR, you'll sequentially apply each byte of the key; the first byte of plaintext will be XOR'd against I, the next C, the next E, then I again for the 4th byte, and so on.

// It should come out to:

// 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272
// a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f

// Encrypt a bunch of stuff using your repeating-key XOR function. Encrypt your mail. Encrypt your password file. Your .sig file. Get a feel for it. I promise, we aren't wasting your time with this.

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include <iomanip> // for std::hex, std::setw, and std::setfill
#include "functions.h"
using namespace std;

int main(){

  char *input = new char[BUFFER_SIZE];
  char *output = new char[BUFFER_SIZE];
  char byte;
  uint sizeBytes = 0;
  
  // Read file line by line
  ifstream file("5.txt");
  string line;

  // Check if the file is opened successfully
  if (!file.is_open()) {
    cerr << "Failed to open the file." << endl;
    return 1;
  }

  // Read file in bytes, and store them as char
  while (file.get(byte)) {
    input[sizeBytes] = byte;
    sizeBytes++;
  }
  sizeBytes--;
  
  //Function has the main logic
  repKeyXOR(input, output, key, sizeBytes, sizeof(key));

  for (uint i=0; i<sizeBytes;i++){
    cout << hex << setw(2) << setfill('0') << uint(output[i]);
  }
  cout << endl;

  return 0;
}