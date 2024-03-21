// An ECB/CBC detection oracle

// Now that you have ECB and CBC working:

// Write a function to generate a random AES key; that's just 16 random bytes.

// Write a function that encrypts data under an unknown key --- that is, a function that generates a random key and encrypts under it.

// The function should look like:

// encryption_oracle(your-input)
// => [MEANINGLESS JIBBER JABBER]

// Under the hood, have the function append 5-10 bytes (count chosen randomly) before the plaintext and 5-10 bytes after the plaintext.

// Now, have the function choose to encrypt under ECB 1/2 the time, and under CBC the other half (just use random IVs each time for CBC). Use rand(2) to decide which to use.

// Detect the block cipher mode the function is using each time. You should end up with a piece of code that, pointed at a block box that might be encrypting ECB or CBC, tells you which one is happening.

// compile using -lcryptopp -L/path/to/cryptopp/lib

#include <iostream>
#include <cstring>
#include <iomanip>
#include "functions.h"
using namespace std;

int main(){
  // unsigned char key[key_size] = {0};
  string input;
  uint outputSize = 0;

  // Input
  cout << "Type the input to encrypt: ";
  getline (cin, input);

  // Convert to a char array
  unsigned char hexInput[input.length()];
  memcpy(hexInput,input.c_str(),sizeof(char)*input.length());

  // Output is max 20 characters longer + padding (max key_size - 1)
  outputSize = input.length() + 20 + key_size;
  unsigned char hexOutput[outputSize];

  // This encrypts and returns the real size of the output on outputSize
  encryption_oracle(hexInput,hexOutput,input.length(),&outputSize);

  return 0;
}