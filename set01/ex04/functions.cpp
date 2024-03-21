// const int BUFFER_SIZE = 100;
// const int TOTAL_ASCII = 256;
// const int REP_LETTERS_SIZE = 20;
// uint repLetters[REP_LETTERS_SIZE] = {'e','o','l','s','t','f','m','p','c','n','E', 'O', 'L', 'S', 'T', 'F', 'M', 'P', 'C', 'N'};

/*
Note: printable ASCII range from 32 (space) to 126 (~), others are not part of English
*/

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include "functions.h"
using namespace std;

/* function to convert string to hex*/
void strToHex(const char* input, uint* result, uint strLen){
  for (uint i=0;i<strLen;i+=2){
    // Variables to traverse the array
    char *temp = new char[2];
    stringstream ssin, ssout;
    uint hexNumber;

    // Array traverse 2 by 2
    memcpy(temp,input+i,2);
    // Convert to hex using string stream
    ssin << hex << temp;
    ssin >> hexNumber;

    // Save array
    result[i>>1] = hexNumber;
    // cout << hexNumber << ",";

    delete[] temp;
  }
}

/* function stringFeatures: finds the frequency of each ASCII character in the string.
   - input: char * to the input, int length of input, vector to store consecutives and uint* to store frequency
      of each ASCII character.
   - output: consecutive vector and uint * to frequency array
*/
void stringFeatures(uint* hexInput, int hexLen, vector<uint>& rep, uint* counters){
  for (uint i=0;i<hexLen-1;i++){
    // counters index is the hex value
    counters[hexInput[i+1]]++;

    // Find consecutives
    if (hexInput[i] == hexInput[i+1]){
      rep.push_back(i);
    }
  }

  // Previous loop did not count the last one.
  counters[hexInput[hexLen-1]]++;
}

/* function stringsAnalysis: based on the features found (consecutive, character frequency and score), tries to 
    find possible keys.
   - input: char array input, uint length of the input, vector for consecutives, uint* for counter of frequencies
            vector pointer for key storage, vector pointer for score storage, vector pointer for key storage 
            based on score.
   - output: vectors and arrays. One vector for consecutive characters, a uint * for the frequency of each character
            and a key vector to store the possible key based on consecutive charactar. Two vectors (score,scoreKey)
            stores the max score for each string and the key that produces it.
*/
void stringsAnalysis(uint* hexInput, uint hexLen, vector<uint> rep, uint* counters, vector<uint>& keys, vector<uint>& score, vector<uint>& scoreKeys){
  uint max = 0, max_i;

  // Consecutives can be from repLetters array in header (functions.h)
  while (!rep.empty()){
    uint index = rep.back();
    rep.pop_back();

    for (uint i=0;i<TOTAL_ASCII;i++){
      if (counters[i]>max){
        max = counters[i];
        max_i = i; //This stores the most repeated character
      }
  }

    for (uint i=0;i<REP_LETTERS_SIZE;i++){
      uint tmpKey = hexInput[index] ^ repLetters[i];
      uint tmpChar = max_i ^ tmpKey;

      if ((tmpChar == ' ') || ((tmpChar > 'A' && tmpChar < 'Z') || (tmpChar > 'a' && tmpChar < 'z'))){
        keys.push_back(tmpKey);
      }
    }
  }

  /* Score based on frequency: we have an array with weights for all ASCII characters, where the index is 
     the number of the ASCII char. 
     We decode each string for all possible keys, while computing the score based on the ranking array in the 
     header file (functions.h), which gives a weight for each ASCII based on its frequency in the English 
     language. We store the maximum score for each string, and the key that produces it. */
  uint maxScore = 0,maxKey;
  /* Try all keys */
  for (uint key=0;key<TOTAL_ASCII;key++){
    uint calcScore = 0;
    for (uint i=0;i<hexLen;i++){
      // Increment score based on ranking
      calcScore += ranking[(hexInput[i] ^ key)];
    }
    if (calcScore>maxScore){
      // But only store the maximum for each string.
      maxScore = calcScore;
      maxKey = key;
    }
  }
  /* At this point we have the one with higher score in maxScore and maxKey*/
  score.push_back(maxScore);
  scoreKeys.push_back(maxKey);
}

/* function validChar: checks if a character is valid.
   - input: char
   - output: boleean. True if th character is valid.
*/
bool validChar(char chr){
  bool result;

  if ((chr > 'A') && (chr < 'Z') && (chr > 'a') && (chr < 'z') && (chr == ' ') && (chr == ',') && (chr == '.') && (chr == '!') && (chr == '?')){
      result = true;
  } else{
      result = false;
  }
  return result;
}

