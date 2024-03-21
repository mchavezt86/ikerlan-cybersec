// Byte-at-a-time ECB decryption (Simple)

// Copy your oracle function to a new function that encrypts buffers under ECB mode using a consistent but unknown key (for instance, assign a single random key, once, to a global variable).

// Now take that same function and have it append to the plaintext, BEFORE ENCRYPTING, the following string:

// Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg
// aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq
// dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg
// YnkK

// Spoiler alert.

// Do not decode this string now. Don't do it.

// Base64 decode the string before appending it. Do not base64 decode the string by hand; make your code do it. The point is that you don't know its contents.

// What you have now is a function that produces:

// AES-128-ECB(your-string || unknown-string, random-key)

// It turns out: you can decrypt "unknown-string" with repeated calls to the oracle function!

// Here's roughly how:

//    (1) Feed identical bytes of your-string to the function 1 at a time --- start with 1 byte ("A"), then "AA", then "AAA" and so on. Discover the block size of the cipher. You know it, but do this step anyway.
//    (2) Detect that the function is using ECB. You already know, but do this step anyways.
//    (3) Knowing the block size, craft an input block that is exactly 1 byte short (for instance, if the block size is 8 bytes, make "AAAAAAA"). Think about what the oracle function is going to put in that last byte position.
//    (4) Make a dictionary of every possible last byte by feeding different strings to the oracle; for instance, "AAAAAAAA", "AAAAAAAB", "AAAAAAAC", remembering the first block of each invocation.
//    (5) Match the output of the one-byte-short input to one of the entries in your dictionary. You've now discovered the first byte of unknown-string.
//    (6) Repeat for the next byte.

// compile with -lcryptopp -L/path/to/cryptopp/lib

/* This code implement this logic.*/

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include "functions.h"
using namespace std;

int main(){
  unsigned char hexArray[4000] = {0}; // Store number as value after base64 decoding.
  
  ifstream file("12.txt");
  string line;
  uint valIndex = 0;
  uint strLen;

  while (getline(file, line)) { //Read each line.
    const char* input = line.c_str();
    strLen = strlen(input);
    decodeBase64(input, strLen, hexArray, &valIndex);
  }
  file.close();
  
  // Global key
  unsigned char key[key_size];
  random_key(key);

  // Output
  unsigned char output[valIndex];

  // Find the block size
  uint block_size = find_block_size(hexArray,valIndex,key);

  aes128ecb_encrypt(hexArray,output,valIndex,key,key_size);

  detect_block_cipher(output,valIndex,block_size);
  
  cout << endl << "Message: " << endl << endl;
//   char c = found_char(hexArray,valIndex,key,block_size);
  for (uint i=0;i<valIndex;i++){
    cout << found_char(&(hexArray[i]),valIndex-i,key,block_size);
  }

  return 0;
}