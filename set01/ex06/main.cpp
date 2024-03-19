// Break repeating-key XOR
// It is officially on, now.

// This challenge isn't conceptually hard, but it involves actual error-prone coding. The other challenges in this set are there to bring you up to speed. This one is there to qualify you. If you can do this one, you're probably just fine up to Set 6.

// There's a file here. It's been base64'd after being encrypted with repeating-key XOR.

// Decrypt it.

// Here's how:

//     Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.
//     Write a function to compute the edit distance/Hamming distance between two strings. The Hamming distance is just the number of differing bits. The distance between:

//     this is a test

//     and

//     wokka wokka!!!

//     is 37. Make sure your code agrees before you proceed.
//     For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, and find the edit distance between them. Normalize this result by dividing by KEYSIZE.
//     The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.
//     Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.
//     Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.
//     Solve each block as if it was single-character XOR. You already have code to do this.
//     For each block, the single-byte XOR key that produces the best looking histogram is the repeating-key XOR key byte for that block. Put them together and you have the key.

// This code is going to turn out to be surprisingly useful later on. Breaking repeating-key XOR ("Vigenere") statistically is obviously an academic exercise, a "Crypto 101" thing. But more people "know how" to break it than can actually break it, and a similar technique breaks something much more important.
// No, that's not a mistake.

// We get more tech support questions for this challenge than any of the other ones. We promise, there aren't any blatant errors in this text. In particular: the "wokka wokka!!!" edit distance really is 37.

#include <iostream>
#include <cstring>
#include <fstream> // File handling
#include <iomanip> // for std::hex, std::setw, and std::setfill
#include <vector>
#include "functions.h"
using namespace std;

int main(){
  
//   cout << "Hamming distance between 't' and 'w': " << hammingByte('t','w') << endl;
//   char s1[] = "this is a test";
//   char s2[] = "wokka wokka!!!";

//   cout << "Hamming distance between " << s1 << " and " << s2 << " is: " << hammingString(s1,s2,14) << endl;

  char hexArray[4000] = {0}; // Store number as value after base64 decoding.

  ifstream file("6.txt");
  string line;
  uint valIndex = 0;
  uint strLen;
  // Vector for the keys
  vector<uint> keyPossibleSize;
  vector<char> finalKeys;
  // Max score for final key calculation
  uint maxScore = 0;
 
  while (getline(file, line)) { //Read each line.
    const char* input = line.c_str();
    strLen = strlen(input);
    decodeBase64(input, strLen, hexArray, &valIndex);
  }

  file.close();
  
//   cout << "Hex array: ";
//   for (uint i=0;i<valIndex;i++){
//     cout << hex << setw(2) << setfill('0') << uint(hexArray[i]);
//   }
//   cout << endl;
  cout << "Lenght in hex: " << valIndex << endl;

//   uint keySize = keySizeCalc(hexArray, valIndex);
  keySizeCalc(hexArray, valIndex, keyPossibleSize);

  while(!keyPossibleSize.empty()){
    uint keySize = keyPossibleSize.back();
    keyPossibleSize.pop_back();
    cout << "Key possible: " << keySize << endl;

    // Score of this key
    uint keyScore = 0;

    uint transLen = valIndex/keySize;

    char newKeys[keySize] = {0};

    for (uint i=0;i<keySize;i++){
      // cout << i << endl;
      char transp[transLen] = {0};
      // Array to count frequencies
      uint counters[TOTAL_ASCII] = {0};
      for (uint j=0;j<transLen;j++){
          transp[j] = hexArray[j*keySize+i];
      }
      stringFeatures(transp,transLen,counters);
      newKeys[i] = stringsAnalysis(transp,transLen,counters);
      // cout << "Key " << i << ": " << newKeys[i] << endl;
    }
    
    for (uint i=0;i<valIndex;i++){
      // cout << char(hexArray[i] ^ newKeys[i%keySize]);
      keyScore += ranking[hexArray[i] ^ newKeys[i%keySize]];
    }
    if (keyScore > maxScore){
      maxScore = keyScore;
      for(uint k=0;k<keySize;k++){
        finalKeys.push_back(newKeys[k]);
      }
    }
    // cout << endl;
  }

  char theFinalKey[finalKeys.size()];
  uint indexKeys = finalKeys.size();
  uint theFinalSize = finalKeys.size();

  while(!finalKeys.empty()){
    indexKeys--;
    theFinalKey[indexKeys] = finalKeys.back();
    finalKeys.pop_back();
    // indexKeys++;
  }

  cout << "\nThe final key is: ";
  for (uint k=0;k<theFinalSize;k++){
    cout << theFinalKey[k];
  }
  cout << endl << endl << "The decoded message is:" << endl << endl;

  for (uint i=0;i<valIndex;i++){
    cout << char(hexArray[i] ^ theFinalKey[i%theFinalSize]);
  }
  cout << endl;

  return 0;
}