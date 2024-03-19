#include <iostream>
#include <cstring>
// #include <sstream>
// #include <vector>
#include <fstream>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include "functions.h"
// #include <crypto++/modes.h>
using namespace std;

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

    // cout << byteValue << endl;

    // Split it into hex
    hexArray[*valIndex] = ((byteValue >> 16) & 0xFF);
    // cout << uint(hexArray[*valIndex] )<< ",";
    (*valIndex)++;
    hexArray[*valIndex] = ((byteValue >> 8) & 0xFF);
    // cout << uint(hexArray[*valIndex]) << ",";
    (*valIndex)++;
    hexArray[*valIndex] = ((byteValue) & 0xFF);
    // cout << uint(hexArray[*valIndex]) << ",";
    (*valIndex)++;
    // cout << endl;
  }
}

void aes128ecb(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte encryptedData[hexSize];// = {0x76, 0x55, 0x0E, 0x47, 0x8D, 0x2F, 0x0B, 0x74, 0x9A, 0x59, 0x38, 0x39, 0xF7, 0x8F, 0x8C, 0xE2};
  CryptoPP::byte key[keyLen];// = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x97, 0x6E, 0x5D, 0x21, 0x47, 0x0E};

  //Fill the data
  for (uint i=0;i<hexSize;i++){
    encryptedData[i] = (CryptoPP::byte)hexArray[i];
  }
  // Fill the key
  for (uint i=0;i<keyLen;i++){
    key[i] = (CryptoPP::byte)theKey[i];
  }

  // Decrtyption data storage and engine
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryption(key, sizeof(key));
  CryptoPP::byte decryptedData[sizeof(encryptedData)];
  
  // The decryption
  decryption.ProcessData(decryptedData, encryptedData, sizeof(encryptedData));

  // Decrypt the data
  decryption.ProcessData(decryptedData, encryptedData, sizeof(encryptedData));


  for (uint i=0;i<hexSize;i++){
    // output[i]=hexArray[i] ^ theKey[i%keyLen];
    // cout << uint(output[i] )<< ", " << uint(hexArray[i]) << ", " << uint(theKey[i%keyLen]) << endl;
    cout << std::hex << static_cast<char>(decryptedData[i]) << " ";
  }
  cout << endl;
}