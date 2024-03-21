#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include "functions.h"
using namespace std;

/* function xor_buffers:
   - input: buffer_1, buffer_2, result buffer and buffer size
   - output: result buffer, with byte-wise XOR of input buffers
*/
void xor_buffers(unsigned char* b1, unsigned char* b2, unsigned char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i++){
    result[i] = b1[i] ^ b2[i];
  }
}

/* function base64Char: gives a number based on the base64 digit.
   - input: char base64 digit
   - output: uint number 
*/
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
    hexArray[*valIndex] = char((byteValue >> 16) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue >> 8) & 0xFF);
    (*valIndex)++;
    hexArray[*valIndex] = char((byteValue) & 0xFF);
    (*valIndex)++;

    /* Check padding */
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

/* function padding: Adds padding to an array.
   - input: char * to the input, uint for the input lenght and uint for block size.
   - output: uint with the final lenght, including padding.
*/
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

/* function aes128ecb_decrypt: decrypts a message encrypted using AES ECB 128
   - input: char * to the input, char * to the ooutput, uint for input size, unsigned char * to the key
    and uint for the key size.
   - output: unsigned char * to the output and its size.
  The implementation is straightforward:
  - Crate variables required by the decryption function, in the format of the CryptoCPP byte class.
  - Instantiate a decription engine (function) using the parameters requried.
  - Call the ProcessData method to decrypt data. 
*/
void aes128ecb_decrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){
  
  // Create CryptoCPP variables
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

  //Copy output
  memcpy(output,decryptedData,sizeof(decryptedData));
}

/* function aes128ecb_encrypt: encrypts a message using AES ECB 128
   - input: char * to the input, char * to the ooutput, uint for input size, unsigned char * to the key
    and uint for the key size.
   - output: unsigned char * to the output.
  The implementation is straightforward:
  - Crate variables required by the decryption function, in the format of the CryptoCPP byte class.
  - Instantiate a decription engine (function) using the parameters requried.
  - Call the ProcessData method to decrypt data. 
*/
void aes128ecb_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen){

  CryptoPP::byte key[keyLen];

  // Fill the key
  memcpy(key,theKey,keyLen);

  // Encrypt the data
  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryption(key, sizeof(key));
  
  // The decryption
  encryption.ProcessData(output, hexArray, hexSize);
}

/* function aes128cbc_encrypt: encrypts a message using AES CBC 128
   - input: char * to the input, char * to the ooutput, uint for input size, unsigned char * to the key,
     uint for the key size and the const char * to the initialization vector
   - output: unsigned char * to the output.
  The implementation is straightforward:
  - Loops the data in blocks with the same size as the key'
  - XORs the current block with the IV, which is the parameter iv for the first round
  - Encrypts using AES 128 ECB and stores it in the output array.
  - Updates the IV.
*/
void aes128cbc_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const unsigned char* iv){
  
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

/* function aes128cbc_decrypt: encrypts a message using AES CBC 128
   - input: char * to the input, char * to the ooutput, uint for input size, unsigned char * to the key,
     uint for the key size and the const char * to the initialization vector
   - output: unsigned char * to the output.
  The implementation is straightforward:
  - Loops the data in blocks with the same size as the key'
  - Decrypts using AES 128 ECB and stores it in a temporal array.
  - XORs the current decrypted block with the IV, which is the parameter iv for the first round
  - Stores the result in the output array.
  - Updates the IV.
*/
void aes128cbc_decrypt(unsigned char* input, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const unsigned char* iv){
  
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