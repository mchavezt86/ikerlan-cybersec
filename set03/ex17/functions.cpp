#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>

// #include <crypto++/aes.h>
// #include <crypto++/modes.h>
// #include <crypto++/filters.h>

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
    
  for (uint i=0;i<pad;i++){
    input[strLen+i] = pad;
  }

  return strLen + pad;
}