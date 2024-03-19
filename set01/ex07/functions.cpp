#include <iostream>
#include <cstring>
// #include <sstream>
// #include <vector>
#include <fstream>
#include "functions.h"
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
//   cout << theKey << endl;
//   for (uint i=0;i<keyLen;i++){
//     cout << theKey[i];
//   }
//   cout << endl;

  for (uint i=0;i<hexSize;i++){
    output[i]=hexArray[i] ^ theKey[i%keyLen];
    cout << uint(output[i] )<< ", " << uint(hexArray[i]) << ", " << uint(theKey[i%keyLen]) << endl;
  }
}