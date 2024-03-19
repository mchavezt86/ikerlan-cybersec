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

unsigned char random_byte(){
  // Seed the random number generator
  random_device rd;
  mt19937 gen(rd()); // Mersenne Twister engine

  // Define the distribution for
  uniform_int_distribution<int> dist(0, 255);

  return (unsigned char)dist(gen);
}

void random_key(unsigned char* key){
  // Seed the random number generator
  random_device rd;
  mt19937 gen(rd()); // Mersenne Twister engine

  // Define the distribution for
  uniform_int_distribution<int> dist(0, 255);
  
  for(uint i=0;i<key_size;i++){
    key[i] = random_byte();
  }
}

void xor_buffers(unsigned char* b1, unsigned char* b2, unsigned char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i++){
    result[i] = b1[i] ^ b2[i];
  }
}

void padding(unsigned char* input, uint* strLen, uint blockSize){
//   cout << blockSize << "," << strLen << endl;
  uint pad;
  if (blockSize / (*strLen) > 0){
    pad = blockSize - (*strLen);
  } else{
    uint div = ((*strLen) / blockSize);
    pad = blockSize - ((*strLen) - (div * blockSize));
    // cout << pad << endl;
  }
    
  for (uint i=0;i<pad;i++){
    input[(*strLen)+i] = pad;
    // cout << pad << endl;
  }

  // Update to real size
  (*strLen)+=pad;
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

void aes128cbc_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, unsigned char* iv){
  
  unsigned char xored[keyLen];
  unsigned char currentIV[keyLen];
  // First IV is the constant iv
  memcpy(currentIV,iv,(sizeof(char)*keyLen));
  for (uint i=0;i<hexSize;i+=keyLen){
    // xor
    xor_buffers(&(hexArray[i]),currentIV,xored,keyLen);
    // encrypt ECB
    aes128ecb_encrypt(xored,&(output[i]),keyLen,theKey,keyLen);
    // update iv
    memcpy(currentIV,&(output[i]),(sizeof(char)*keyLen));
  }
}

void extra_bytes(unsigned char* hexInput,unsigned char* hexOutput, uint inputSize, uint* outputSize){
  // Random 5 - 10
  uint extra = 5 + random_byte() % 5;
  *outputSize = inputSize + extra;

  // First append
  for (uint i=0;i<extra;i++){
    hexOutput[i] = random_byte();
  }

  // Data
  memcpy(&(hexOutput[extra]),hexInput,inputSize);

  // Random 5 - 10 and second append
  extra = 5 + random_byte() % 5;

  for (uint i=0;i<extra;i++){
    hexOutput[*outputSize+i] = random_byte();
  }
  *outputSize += extra;
}

bool repeated(unsigned char* s1, unsigned char* s2, uint total){

    for (uint i=0;i<total;i++){
      if ((s1[i] ^ s2[i]) != 0){
        return false;
      }
    }
    return true;
}

void detect_block_cipher(unsigned char* input,uint outputSize, uint key_size){
  bool ECB = false;
  for (uint i=0;i<outputSize-key_size;i+=key_size){
    for (uint j=i+key_size;j<outputSize;j+=key_size){
      if ( repeated(&(input[i]),&(input[j]),key_size) ){
        ECB = true;
        break;
      }
    }
    if (ECB){
      break;
    }
  }

  if (ECB){
    cout << "This was using ECB" << endl;
  } else{
    cout << "This was using CBC" << endl;
  }
}

void encryption_oracle(unsigned char* hexInput,unsigned char* hexOutput, uint inputSize, uint* outputSize){
  
  unsigned char tempArray[(*outputSize)] = {0};
  unsigned char key[key_size] = {0};

  // Adding extra bytes
  extra_bytes(hexInput, tempArray, inputSize, outputSize);

  // Padding
  padding(tempArray, outputSize, key_size);
  // memcpy(hexOutput,tempArray,(*outputSize));

  // Random key
  random_key(key);

  uint luck = random_byte() % 2;

  if (luck == 1){
    cout << "(shh: ECB)" << endl;
    aes128ecb_encrypt(tempArray,hexOutput,(*outputSize),key,key_size);
  }else{
    cout << "(shh: CBC)" << endl;
    unsigned char iv[key_size];
    random_key(iv);
    aes128cbc_encrypt(tempArray,hexOutput,(*outputSize),key,key_size,iv);
  }

  cout << "Oracle will detect which encryption was used..." << endl;

  detect_block_cipher(hexOutput,(*outputSize),key_size);

}