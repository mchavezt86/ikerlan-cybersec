#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include "functions.h"
using namespace std;

void xor_buffers(unsigned char* b1, unsigned char* b2, char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i+=2){
    //Variables to traverse the array
    char *temp1 = new char[2];
    char *temp2 = new char[2];
    stringstream ss1, ss2, ssxor;
    uint hex1, hex2, hexXor;

    // Array traverse 2 by 2
    memcpy(temp1,b1+i,2);
    memcpy(temp2,b2+i,2);

    // Convert to hex using string stream
    ss1 << hex << temp1;
    ss2 << hex << temp2;

    ss1 >> hex1;
    ss2 >> hex2;

    hexXor = hex1 ^ hex2;

    ssxor << hex << hexXor;

    // cout << ssxor.str().c_str();
    memcpy(result+i,ssxor.str().c_str(),2);

    delete[] temp1;
    delete[] temp2;
  }
}

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

    // Split it into hex
    hexArray[*valIndex] = char((byteValue >> 16) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue >> 8) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue) & 0xFF);
    (*valIndex)++;
  }
  for (uint i=0;i<*valIndex;i++){
    hexArray[i]=(hexArray[i] & 0xFF);
  }
}

void padding(unsigned char* input, uint strLen, uint blockSize){
//   cout << blockSize << "," << strLen << endl;
  uint pad;
  if (blockSize / strLen > 0){
    pad = blockSize - strLen;
  } else{
    uint div = (strLen / blockSize);
    pad = blockSize - (strLen - (div * blockSize));
    // cout << pad << endl;
  }
    
  for (uint i=0;i<pad;i++){
    input[strLen+i] = pad;
    // cout << pad << endl;
  }
}

void aes128ecb_decrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte encryptedData[hexSize];// = {0x76, 0x55, 0x0E, 0x47, 0x8D, 0x2F, 0x0B, 0x74, 0x9A, 0x59, 0x38, 0x39, 0xF7, 0x8F, 0x8C, 0xE2};
  CryptoPP::byte key[keyLen];// = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x97, 0x6E, 0x5D, 0x21, 0x47, 0x0E};

  // Fill the data
  memcpy(encryptedData,hexArray,hexSize);

  for (uint i=0;i<hexSize;i++){
    cout << uint(encryptedData[i]) << ",";
  }
  cout << endl;

  // Fill the key
  memcpy(key,theKey,keyLen);

  // Decrtyption data storage and engine
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryption(key, sizeof(key));
  CryptoPP::byte decryptedData[sizeof(encryptedData)];
  
  // The decryption
  decryption.ProcessData(decryptedData, encryptedData, sizeof(encryptedData));
  // for (uint i=0;i<hexSize;i+=sizeof(key)){
  //   decryption.ProcessData(&decryptedData[i], &encryptedData[i], sizeof(key));
  // }

  for (uint i=0;i<hexSize;i++){
    // output[i]=hexArray[i] ^ theKey[i%keyLen];
    // cout << uint(output[i] )<< ", " << uint(hexArray[i]) << ", " << uint(theKey[i%keyLen]) << endl;
    output[i] = static_cast<unsigned char>(decryptedData[i]);
    // cout << std::hex << static_cast<char>(decryptedData[i]) << " ";
    cout << decryptedData[i];
  }
  cout << endl;
}

void aes128ecb_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte key[keyLen];// = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x97, 0x6E, 0x5D, 0x21, 0x47, 0x0E};

  // Fill the key
  memcpy(key,theKey,keyLen);

  // Encrypt the data
  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryption(key, sizeof(key));
  
  // The decryption
  encryption.ProcessData(output, hexArray, hexSize);
  // for (uint i=0;i<hexSize;i+=sizeof(key)){
  //   decryption.ProcessData(&decryptedData[i], &encryptedData[i], sizeof(key));
  // }

  for (uint i=0;i<hexSize;i++){
    // output[i]=hexArray[i] ^ theKey[i%keyLen];
    // cout << uint(output[i] )<< ", " << uint(hexArray[i]) << ", " << uint(theKey[i%keyLen]) << endl;
    // output[i] = static_cast<unsigned char>(output[i]);
    // cout << std::hex << static_cast<char>(decryptedData[i]) << " ";
    cout << uint(output[i]) << ",";
  }
  cout << endl;
}