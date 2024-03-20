// Implement CTR, the stream cipher mode

// The string:

// L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==

// ... decrypts to something approximating English in CTR mode, which is an AES block cipher mode that turns AES into a stream cipher, with the following parameters:

//       key=YELLOW SUBMARINE
//       nonce=0
//       format=64 bit unsigned little endian nonce,
//              64 bit little endian block count (byte count / 16)

// CTR mode is very simple.

// Instead of encrypting the plaintext, CTR mode encrypts a running counter, producing a 16 byte block of keystream, which is XOR'd against the plaintext.

// For instance, for the first 16 bytes of a message with these parameters:

// keystream = AES("YELLOW SUBMARINE",
//                 "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00")

// ... for the next 16 bytes:

// keystream = AES("YELLOW SUBMARINE",
//                 "\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00")

// ... and then:

// keystream = AES("YELLOW SUBMARINE",
//                 "\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00")

// CTR mode does not require padding; when you run out of plaintext, you just stop XOR'ing keystream and stop generating keystream.

// Decryption is identical to encryption. Generate the same keystream, XOR, and recover the plaintext.

// Decrypt the string at the top of this function, then use your CTR function to encrypt and decrypt other things. 

// compile with -lcryptopp -L/path/to/cryptopp/lib

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include <iomanip> // Pretty printing
#include "functions.h"
using namespace std;

int main(){
//   uint str_len64 = sizeof(input);
//   unsigned char input_base64[str_len64];
//   memcpy(input_base64,input,(sizeof(char))*str_len64);

  unsigned char theKey[key_size];
  memcpy(theKey,key,(sizeof(char))*key_size);

//   cout << "Flag 1" << endl;

  ctr_mode(theKey);
  
//   cout << "Flag 2" << endl;
  
  return 0;
}