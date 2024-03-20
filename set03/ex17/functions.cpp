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

  for (uint i=0;i<*valIndex;i++){
    hexArray[i]=(hexArray[i] & 0xFF);
  }
}

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

uint padding(unsigned char* input, uint strLen, uint blockSize){

  uint pad;
  if (blockSize / strLen > 0){
    pad = blockSize - strLen;
  } else{
    uint div = (strLen / blockSize);
    pad = blockSize - (strLen - (div * blockSize));
    // cout << pad << endl;
  }
  if (pad == 0){
    pad = blockSize;
  }

  for (uint i=0;i<pad;i++){
    input[strLen+i] = pad;
  }
  return strLen + pad;
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

void aes128cbc_decrypt(unsigned char* input, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, unsigned char* iv){
  
  unsigned char tmp[keyLen];
  unsigned char currentIV[keyLen];
  // First IV is the constant iv
  memcpy(currentIV,iv,(sizeof(char)*keyLen));
  for (uint i=0;i<hexSize;i+=keyLen){
    // decrypt ECB
    aes128ecb_decrypt(&(input[i]),tmp,keyLen,theKey,keyLen);
    // xor
    xor_buffers(tmp,currentIV,&(output[i]),keyLen);
    // update iv
    memcpy(currentIV,&(input[i]),(sizeof(char)*keyLen));
  }
}

void fullAES(unsigned char* output, unsigned char* key, unsigned char* iv, uint* full_size){
  unsigned char hexArray[max_size] = {0}; // Store number as value after base64 decoding.
  
  ifstream file("17.txt");
  string line;
  uint valIndex = 0;
  uint strLen;
  
  // Generate key
//   unsigned char key[key_size];
  random_key(key);

  // Generate random number for row between 0 and 9
  uint row = uint(random_byte()) % 10;
  uint row_counter = 0;
  
  // Read random string
  while (getline(file, line)) { //Read each line.
    if (row_counter == row){
      const char* input = line.c_str();
      strLen = strlen(input);
      decodeBase64(input, strLen, hexArray, &valIndex);
      break;
    }
    row_counter++;
  }
  file.close();

  // Actual size
  valIndex = padding(hexArray, valIndex, key_size);

  // Random IV
  random_key(iv);

  // encryption
  aes128cbc_encrypt(hexArray,output,valIndex,key,key_size,iv);

  //Update for return
  (*full_size) = valIndex;

//   for (uint i=0;i<valIndex;i++){
//     cout << uint(hexArray[i]) << ",";
//   }
//   cout << endl;
  
}

bool padding_ok(unsigned char* decrypted, uint full_size){
    uint pad = (uint) decrypted[full_size-1];
    // cout << "Pad: " << pad << endl;
    bool result = true;

    for (uint i=0;i<pad;i++){       
      if ((uint)decrypted[full_size-1-i] != pad){
        // cout << (uint)decrypted[full_size-1-i] << endl;
        result = false;
        break;
        }
    }
    return result;
}

bool padding_check(unsigned char* output,unsigned char* key, unsigned char* iv, uint full_size){
  unsigned char decrypted[max_size];

//   cout << "Size: " << full_size << endl;
  
  aes128cbc_decrypt(output,decrypted,full_size,key,key_size,iv);

//   for (uint i=0;i<full_size;i++){
//     cout << uint(decrypted[i]) << ",";
//   }
//   cout << endl;

  return padding_ok(decrypted,full_size);
}