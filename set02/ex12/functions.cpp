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

/* function xor_buffers:
   - input: buffer_1, buffer_2, result buffer and buffer size
   - output: result buffer, with byte-wise XOR of input buffers
*/
void xor_buffers(unsigned char* b1, unsigned char* b2, unsigned char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i++){
    result[i] = b1[i] ^ b2[i];
  }
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


uint find_block_size(unsigned char* hexArray,uint valIndex, unsigned char* key){
  
  unsigned char test = 'A';

  for(uint i=2;i<40;i+=2){
    // cout << i << endl;
    unsigned char input[i+valIndex];
    unsigned char output[i+valIndex];

    // Fill the array with test
    memset(input,test,sizeof(char)*i);
    // and then the array itself
    memcpy(&(input[i]),hexArray,sizeof(char)*valIndex);

    // Encrypt
    aes128ecb_encrypt(input,output,i+valIndex,key,key_size);

    // Xor the first two blocks and add
    unsigned char xored[i];
    uint xored_sum = 0;

    xor_buffers(output,&(output[i/2]),xored,i/2);

    for (uint k=0;k<i/2;k++){
    //   cout << i << ":" << uint(output[k]) << "," << uint(output[i+k]) << endl;
      xored_sum += uint(xored[k]);
    }

    // cout << xored_sum << endl << endl;

    if (xored_sum == 0){
      cout << "Found block size to be " << i/2 << endl;
      return i/2;
    }
  }
  return 0;
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

char found_char(unsigned char* input,uint input_size, unsigned char* key, uint block_size){
  unsigned char test = 'A';
  // To compare
  unsigned char byteshort_ref[block_size];
  unsigned char block_dict[256*block_size];
  
  // inputs
  unsigned char short_input[input_size + block_size - 1];
  unsigned char test_input[input_size + block_size];

  // outputs
  unsigned char short_output[input_size + block_size - 1];
  unsigned char test_output[input_size + block_size];

  // Fill short input  
  memset(short_input,test,sizeof(char)*(block_size-1));
  memcpy(&(short_input[block_size-1]),input,sizeof(char)*input_size);
  
  // Encrypt the short
  aes128ecb_encrypt(short_input,short_output,input_size + block_size - 1,key,key_size);
  memcpy(byteshort_ref,short_output,sizeof(char)*block_size);
  
  // Fill test inputs
  for (uint i=0;i<256;i++){
    memset(test_input,test,sizeof(char)*(block_size-1));
    memset(&(test_input[block_size-1]),char(i),sizeof(char));
    memcpy(&(test_input[block_size]),input,sizeof(char)*input_size);

    // Encrypt the test
    aes128ecb_encrypt(test_input,test_output,input_size + block_size,key,key_size);
    memcpy(&(block_dict[i*block_size]),test_output,sizeof(char)*block_size);

    // cout << "Char under test is " << i << endl;
  }
//   cout << "Finish?" << endl;
  
  // Analyse dict
  for (uint i=0;i<256;i++){
    uint xored_sum = 0;
    unsigned char xored[block_size];

    xor_buffers(byteshort_ref,&(block_dict[i*block_size]),xored,sizeof(char)*block_size);

    for(uint k=0;k<block_size;k++){
      xored_sum += uint(xored[k]);
    }
    if(xored_sum==0){
    //   cout << "Char is " << char(i) << endl;
      return char(i);
    }
  }

return 0;
}