#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include "functions.h"
using namespace std;

uint base64Char(char c){
  uint value;
  if ((c >= 'A') && (c <= 'Z')){
    value = c - 'A';
  } else if ((c >= 'a') && (c <= 'z')){
    value = c - 'a' + 26;
  } else if ((c >= '0') && (c <= '9')){
    value = c - '0' + 52;
  } else if (c == '+'){
    value = 62;
  } else if (c == '/'){
    value = 63;
  } else{
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

    // Split it into hex
    hexArray[*valIndex] = char((byteValue >> 16) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue >> 8) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue) & 0xFF);
    (*valIndex)++;

    if(input[i+2]=='='){
        (*valIndex)--;
    }
    if(input[i+3]=='='){
        (*valIndex)--;
    }
  }

  for (uint i=0;i<(*valIndex);i++){
    hexArray[i]=(hexArray[i] & 0xFF);
  }
}

void xor_buffers(unsigned char* b1, unsigned char* b2, unsigned char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i++){
    result[i] = b1[i] ^ b2[i];
  }
}

void aes128ecb_decrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte encryptedData[hexSize];// = {0x76, 0x55, 0x0E, 0x47, 0x8D, 0x2F, 0x0B, 0x74, 0x9A, 0x59, 0x38, 0x39, 0xF7, 0x8F, 0x8C, 0xE2};
  CryptoPP::byte key[keyLen];// = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x97, 0x6E, 0x5D, 0x21, 0x47, 0x0E};

  // Fill the data
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
  }
//   cout << endl;
}

void aes128ecb_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte key[keyLen];// = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x97, 0x6E, 0x5D, 0x21, 0x47, 0x0E};

  // Fill the key
  memcpy(key,theKey,keyLen);

  // Encrypt the data
  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryption(key, sizeof(key));
  
  // The decryption
  encryption.ProcessData(output, hexArray, hexSize);
}

void counter_format(uint counter, unsigned char* counter_array){
  counter = counter & 0xFFFFFFFFFFFFFFFF;

  for (uint i=0;i<key_size/2;i++){
    counter_array[i] = counter & 0xFF;
    counter = counter >> 8;
  }
}

void ctr_decrypt(unsigned char* hexArray, uint hexSize, unsigned char* output, unsigned char* theKey, unsigned char* nonce){
  uint counter=0;
  unsigned char counter_array[key_size/2];
  unsigned char format[key_size];
  unsigned char temp[key_size];

  // copy the first half of the key which is the nonce
  memcpy(format,nonce,sizeof(char)*key_size/2);

  // copy the counter
  for (uint i=0;i<hexSize;i+=key_size){
    counter_format(counter,counter_array);
    memcpy(&(format[key_size/2]), counter_array, sizeof(char)*key_size/2);
    
    // for (uint k=0;k<key_size;k++){
    //   cout << uint(format[k]) << ",";
    // }
    // cout << endl;

    // Decrypt == Encryp for CTR. 
    aes128ecb_encrypt(format,temp,key_size,theKey,key_size);

    //XOR with Ciphertext / Plaintext
    xor_buffers(&(hexArray[i]),temp,&(output[i]),key_size);

    //Print for now
    for(uint k=0;k<key_size;k++){
      cout << output[i+k];
    }

    counter++;
   }
   cout << endl;
}

void ctr_mode(unsigned char* theKey){
  unsigned char hexArray[400];
  uint hexSize=0;
  uint str_len64 = strlen(input);

  decodeBase64(input,str_len64,hexArray,&(hexSize));

  cout << "Hex size is: " << hexSize << endl;

  unsigned char output[hexSize];

  unsigned char nonce[key_size/2] = {0};

  ctr_decrypt(hexArray, hexSize, output, theKey, nonce);

}