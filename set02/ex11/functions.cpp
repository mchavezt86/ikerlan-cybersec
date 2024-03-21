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

/* function random_byte:
   - input: none
   - output: a randon unsigned char.
*/
unsigned char random_byte(){
  // Seed the random number generator
  random_device rd;
  mt19937 gen(rd()); // Mersenne Twister engine

  // Define the distribution for
  uniform_int_distribution<int> dist(0, 255);

  return (unsigned char)dist(gen);
}

/* function random_key:
   - input: char * to the key
   - output: char * to the key
   The implementation loops the kay until the size defined by the global key_size and stores a random
   byte to the key array.
*/
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

/* function xor_buffers:
   - input: buffer_1, buffer_2, result buffer and buffer size
   - output: result buffer, with byte-wise XOR of input buffers
*/
void xor_buffers(unsigned char* b1, unsigned char* b2, unsigned char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i++){
    result[i] = b1[i] ^ b2[i];
  }
}

/* function padding: Adds padding to an array.
   - input: char * to the input, uint for the input lenght and uint for block size.
   - output: uint with the final lenght, including padding.
*/
void padding(unsigned char* input, uint* strLen, uint blockSize){
//   cout << blockSize << "," << strLen << endl;
  uint pad;
  if (blockSize / (*strLen) > 0){
    pad = blockSize - (*strLen);
  } else{
    // uint div = (strLen / blockSize);
    // pad = blockSize - (strLen - (div * blockSize));
    pad = strLen % blockSize;
  }
    
  for (uint i=0;i<pad;i++){
    input[(*strLen)+i] = pad;
  }
  // Update to real size
  (*strLen)+=pad;
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

/* function extra_bytes: adds extra random bytes to the input array at the beginning and at the end.
   - input: char * to the input, char * to the output, uint input size and uint * to the output size
   - output: char * to the output and its size
*/
void extra_bytes(unsigned char* hexInput, unsigned char* hexOutput, uint inputSize, uint* outputSize){
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

/* function repeated: Finds if two char arrays have the same data.
   - input: two char * to the inputs, uint for their size
   - output: boolean, true if the are repeated
*/
bool repeated(unsigned char* s1, unsigned char* s2, uint total){

    for (uint i=0;i<total;i++){
      if ((s1[i] ^ s2[i]) != 0){
        return false;
      }
    }
    return true;
}

/* function detect_block_cipher: Finds which block cipher was used ECB or CBC
   - input: char * to the input, uint for their size, uint size of the key (block)
   - output: prints which one was used
  The implementation uses the repeated block approach: if a block is repeated the block cipher
  should be ECB. The code iterates the input in bloacks with the same size as the key. If a 
  repetition is found, prints ECB and if not prints CBC.
*/
void detect_block_cipher(unsigned char* input,uint inputSize, uint key_size){
  bool ECB = false;
  for (uint i=0;i<inputSize-key_size;i+=key_size){
    for (uint j=i+key_size;j<outpinputSizeutSize;j+=key_size){
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