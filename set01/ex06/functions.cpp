#include <iostream>
#include <cstring>
#include <sstream>
// #include <vector>
#include <fstream>
#include "functions.h"
using namespace std;

/* function hammingByte: computes the Hamming distance between two chars.
   - input: char input 1 and input 2
   - output: uint Hamming distance
*/
uint hammingByte(char c1, char c2){
  uint xor_val = uint(c1 ^ c2);
  uint distance = 0;
  /* Count the number of ones in a byte after XORing*/
  for(uint i=0;i<8;i++){
    distance += (xor_val & 1);
    xor_val = xor_val >> 1;
  }
  return distance;
}

/* function hammingString: computes the Hamming distance between two strings.
   - input: unsigned char* input 1 and input 2, and string size.
   - output: uint Hamming distance
*/
uint hammingString(unsigned char* s1, unsigned char* s2, uint strSize){
  uint total = 0;
  for(uint i=0;i<strSize;i++){
    // We compute byte by byte and accumulate each iteration.
    total += hammingByte(s1[i], s2[i]);
  }
  return total;
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

  /* Four Base64 digits account for three bytes. */
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

/* Based on the Hamming distance */
/* function keySizeCalc: finds the possible key sizes based on the Hamming distance of "key size" blocks.
   - input: char * to the input, uint for array size, and vector with possible key sizes.
   - output: vector with possible key sizes.
*/
uint keySizeCalc(unsigned char* hexArray, uint valIndex, vector<uint>& keyPossible){

  uint sumAvg[N_TMP_KEY_SIZE] = {0};
  
  /* Loop all possible key sizes until the one defined in the header file */
  for (uint tmpKeySize=2;tmpKeySize<N_TMP_KEY_SIZE;tmpKeySize++){
    for (uint nKeySize=0;nKeySize<KEYSIZE_AVG;nKeySize++){
        sumAvg[tmpKeySize] += hammingString(&hexArray[nKeySize*tmpKeySize],&hexArray[(nKeySize+1)*tmpKeySize],tmpKeySize);
    }
    /* Dealing with integers could lead to potential loss of information when averaging. */
    sumAvg[tmpKeySize] = sumAvg[tmpKeySize]/KEYSIZE_AVG*100;
    sumAvg[tmpKeySize] = sumAvg[tmpKeySize]/tmpKeySize/100;
  }

  /* Find the minimum Hamming distance*/
  uint min = 1000;
  uint minKey;
  for (uint i=2;i<N_TMP_KEY_SIZE;i++){
    if (sumAvg[i]<min){
        min = sumAvg[i];
        minKey = i;
    }
  }
  
  /* However, maybe more than one key size satisfy the minimum. We store all the key sizes that 
    produces the minimum Hamming distance. */
  for (uint i=2;i<N_TMP_KEY_SIZE;i++){
    if (sumAvg[i] == min){
        keyPossible.push_back(i);
    }
  }
  return minKey;
}

void stringFeatures(unsigned char* hexInput, int hexLen, uint* counters){
  for (uint i=0;i<hexLen;i++){
    // counters index is the hex value
    counters[uint(hexInput[i]) & 0xFF]++;
  }
}

/* function stringsAnalysis: based on the features found (character frequency and score), tries to a find a key.
   - input: char array input, uint length of the input, uint* for counter of frequencies (last one not used).
   - output: directly outputs the key that produces the maximum score.
*/
char stringsAnalysis(unsigned char* hexInput, uint hexLen, uint* counters){
  /* Score based on frequency: we have an array with weights, where the index is the number of the ASCII char. 
  For earch decoded value, we use  */
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