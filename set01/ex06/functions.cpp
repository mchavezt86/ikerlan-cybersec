#include <iostream>
#include <cstring>
#include <sstream>
// #include <vector>
#include <fstream>
#include "functions.h"
using namespace std;

uint hammingByte(char c1, char c2){
  uint xor_val = uint(c1 ^ c2);
  uint distance = 0;
  for(uint i=0;i<8;i++){
    distance += (xor_val & 1);
    xor_val = xor_val >> 1;
  }
  return distance;
}

uint hammingString(char* s1, char* s2, uint strSize){
  uint total = 0;
  for(uint i=0;i<strSize;i++){
    total += hammingByte(s1[i], s2[i]);
  }
  return total;
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

void decodeBase64(const char* input, uint strLen, char* hexArray, uint* valIndex){
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

/* Based on the Hamming distance */
uint keySizeCalc(char* hexArray, uint valIndex, vector<uint>& keyPossible){

  uint sumAvg[N_TMP_KEY_SIZE] = {0};
  
  for (uint tmpKeySize=2;tmpKeySize<N_TMP_KEY_SIZE;tmpKeySize++){
    for (uint nKeySize=0;nKeySize<KEYSIZE_AVG;nKeySize++){
        sumAvg[tmpKeySize] += hammingString(&hexArray[nKeySize*tmpKeySize],&hexArray[(nKeySize+1)*tmpKeySize],tmpKeySize);
    }
    sumAvg[tmpKeySize] = sumAvg[tmpKeySize]/KEYSIZE_AVG*100;
    sumAvg[tmpKeySize] = sumAvg[tmpKeySize]/tmpKeySize/100;
  }

  uint min = 1000;
  uint minKey;
  for (uint i=2;i<N_TMP_KEY_SIZE;i++){
    // cout << "Key size " << i << " average: " << sumAvg[i] << endl;
    if (sumAvg[i]<min){
        min = sumAvg[i];
        minKey = i;
    }
  }
  
  for (uint i=2;i<N_TMP_KEY_SIZE;i++){
    if (sumAvg[i] == min){
        keyPossible.push_back(i);
    }
  }

  return minKey;
}

/* function to find features of the string*/
void stringFeatures(char* hexInput, int hexLen, uint* counters){
  for (uint i=0;i<hexLen;i++){
    // counters index is the hex value
    counters[uint(hexInput[i]) & 0xFF]++;
  }
}

/* function to check if consecutives have information*/
char stringsAnalysis(char* hexInput, uint hexLen, uint* counters){
  /* Score based on frequency: we have an array with weights, where the index is the number of the ASCII char. For earch decoded value, we use  */
  uint maxScore = 0;
  char maxKey;
  /* Try all keys */
  for (uint key=0;key<TOTAL_ASCII;key++){
    uint calcScore = 0;
    for (uint i=0;i<hexLen;i++){
      uint index = ((uint) hexInput[i]) ^ key;
      index = index & 0xFF;
    //   cout << "ranking index: " << index << endl;
      calcScore += ranking[index]; //uint(hexInput[i] ^ key) & 0xFF
    }
    // cout << "calcScore"  << calcScore << endl;
    if (calcScore>maxScore){
      maxScore = calcScore;
      maxKey = key & 0xFF;
    }
  }
//   cout << "A2.2" << endl;
  return maxKey;
}