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

/* function to find features of the string*/
void stringFeatures(uint* hexInput, int hexLen, vector<uint>& rep, uint* counters){
  for (uint i=0;i<hexLen-1;i++){
    // counters index is the hex value
    counters[hexInput[i+1]]++;

    // Find consecutives
    if (hexInput[i] == hexInput[i+1]){
      rep.push_back(i);
      // Test for
    //   cout << "Repetition at: " << i << endl;
    }
  }

  // Previous loop did not count the last one.
  counters[hexInput[hexLen-1]]++;
}

/* function to check if consecutives have information*/
void stringsAnalysis(uint* hexInput, uint hexLen, vector<uint> rep, uint* counters, vector<uint>& keys, vector<uint>& score, vector<uint>& scoreKeys){
  uint max = 0, max_i;

  // Consecutives can be from repLetters
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

  /* Score based on frequency: we have an array with weights, where the index is the number of the ASCII char. For earch decoded value, we use  */
  uint maxScore = 0,maxKey;
  /* Try all keys */
  for (uint key=0;key<TOTAL_ASCII;key++){
    uint calcScore = 0;
    for (uint i=0;i<hexLen;i++){
      calcScore += ranking[(hexInput[i] ^ key)];
    }
    if (calcScore>maxScore){
      maxScore = calcScore;
      maxKey = key;
    }
  }
  /* At this point we have the one with higher score in maxScore and maxKey*/
  score.push_back(maxScore);
  scoreKeys.push_back(maxKey);
}

bool validChar(char chr){
  bool result;

  if ((chr > 'A') && (chr < 'Z') && (chr > 'a') && (chr < 'z') && (chr == ' ') && (chr == ',') && (chr == '.') && (chr == '!') && (chr == '?')){
      result = true;
  } else{
      result = false;
  }
  return result;
}

