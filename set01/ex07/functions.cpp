#include <iostream>
#include <cstring>
// #include <sstream>
// #include <vector>
#include <fstream>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include "functions.h"
using namespace std;

/* function base64Char: gives a number based on the base64 digit.
   - input: char base64 digit
   - output: uint number 
*/
uint base64Char(unsigned char c){
  uint value;
  if ((c >= 'A') && (c <= 'Z')){
    value = c - 'A';
  } else if ((c >= 'a') && (c <= 'z')){
    value = c + 26 - 'a';
  } else if ((c >= '0') && (c <= '9')){
    value = c + 52 - '0';
  } else if (c == '+'){
    value = 62;
  } else if (c == '/'){
    value = 63;
  } else {
    value = 0;
  }

  return value;
}

/* function decodeBase64: decodes a base64 input into a hex (number) array
   - input: char * to the input, uint for input size, unsigned char * to output and uint * to
    the final size of the output.
   - output: unsigned char * to the output and its size.
*/
void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex){
    /* Check the padding.*/
  uint nvalues = strLen * 3 / 4;

  for (uint i=0;i<strLen;i+=4){
    uint byteValue = 0;
    //Decode the value
    byteValue += (base64Char(input[i]) << 18) & 0xFC0000;
    byteValue += (base64Char(input[i+1]) << 12) & 0x03F000 ;
    byteValue += (base64Char(input[i+2]) << 6) & 0x000FC0;
    byteValue += (base64Char(input[i+3])) & 0x00003F;

    // Split it into hex
    hexArray[*valIndex] = ((byteValue >> 16) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = ((byteValue >> 8) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = ((byteValue) & 0xFF);
    (*valIndex)++;

    /* Check padding */
    if(input[i+2]=='='){
        (*valIndex)--;
    }
    if(input[i+3]=='='){
        (*valIndex)--;
    }
  }
}

void aes128ecb(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte encryptedData[hexSize];
  CryptoPP::byte key[keyLen];

  //Fill the data
  memcpy(encryptedData,hexArray,hexSize);

  // Fill the key
  memcpy(key,theKey,keyLen);

  // Decrtyption data storage and engine
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryption(key, sizeof(key));
  CryptoPP::byte decryptedData[sizeof(encryptedData)];
  
  // The decryption
  decryption.ProcessData(decryptedData, encryptedData, sizeof(encryptedData));

  for (uint i=0;i<hexSize;i++){
    output[i] = static_cast<unsigned char>(decryptedData[i]);
    cout << output[i];
  }
  cout << endl;
}