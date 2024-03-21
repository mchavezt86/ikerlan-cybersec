// Detect single-character XOR

// One of the 60-character strings in this file has been encrypted by single-character XOR.

// Find it.

// (Your code from #3 should help.)

/* The code from the previous Challenge 3 is not useful due to the logic used in that challenge: repetition of
    one character. This is not generic, as we are now presented with a set of strings that do not necessary
    have repeated characters (and are valid) or strings that have repeated characters but are valid.
    
    The logic used for this challenge is to try the consecutive loop around all possible keys. For each key, the decoded string is
    ranked based on an array (defined in the header file functions.h) which gives to each ASCII character a 
    score based on its frequency on the English language
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <bits/stdc++.h> //Vector functions
#include <fstream> // File handling
#include "functions.h"
using namespace std;

int main(){
  // Input variable
  uint *hexInput = new uint[BUFFER_SIZE/2];

  // Array to count frequencies
  uint counters[TOTAL_ASCII] = {0};

  // Vector to store repetitions
  vector<uint> repetitions;

  // Length of varialbles
  uint strLen;
  uint hexLen;

  // Store possible keys from repetitions
  vector<uint> repKeys;

  // Store scores in a vector
  vector<uint> scores;

  // Store the keys for each score
  vector<uint> scoreKeys;

  // What scored-based analysis: we want the max score and the key that produces it.
  uint maxScore;
  uint maxIndex;
  uint maxKey;

  // Number of lines
  uint nlines = 0;

  // Repetitions worked?
  uint repRound = 0;
  
  // Read file line by line
  ifstream file("4.txt");
  string line;

  // Check if the file is opened successfully
  if (!file.is_open()) {
      cerr << "Failed to open the file." << endl;
      return 1;
  }

  // Read lines from the file until the end
  /* The previous analaysis works only if there are repeated characters. We keep this but will add a 
  second analysis if this one fails. */
  while (getline(file, line)) {
    const char* input = line.c_str();
    strLen = strlen(input);
    hexLen = strLen/2;
    bool correctness = false;
    uint finalKey;

    // Number of lines
    nlines++;

    // Convert string input to hex
    strToHex(input, hexInput, strLen);

    // Extract features of string: character count and consecutives characters.
    stringFeatures(hexInput, hexLen, repetitions, counters);

    // Analysis of the features, keys stores the possible keys.
    /* Aside from the consecutive approach, we store the (max) score for each string and the key
      that produces this max score. */
    stringsAnalysis(hexInput, hexLen, repetitions, counters, repKeys, scores, scoreKeys);

    /* Try the consecutive character approach, but check if the decoded result has only valid 
      characters (letters, puntuacion, spaces)*/    
    while(!repKeys.empty()){
        finalKey = repKeys.back();
        repKeys.pop_back();
        correctness = true;
        for (uint i=0;i<hexLen;i++){
          uint tmpVal = hexInput[i] ^ finalKey;
          if (!validChar(tmpVal)){
            correctness = false;
            break;
          }
          if (correctness == false){
            break;
          }
        }
    }

    if (correctness == true){
      repRound++;
      for (uint i=0;i<hexLen;i++){
        cout << char(hexInput[i] ^ finalKey);
      }
      cout << endl;
    }
  }

  /* Consectuive approach leads to nowhere, let's try with the max score per string (line from file)*/
  if (repRound == 0){
    maxScore = *max_element(scores.begin(), scores.end());
    maxIndex = distance(scores.begin(),find(scores.begin(),scores.end(),maxScore));
    maxKey = scoreKeys[maxIndex];
    cout << maxScore << " is the max score at " << maxIndex << endl;
  }

  file.close();

  // Read file line by line
  file.clear();
  file.open("4.txt");

  nlines = 0;

  // Loop to print with the string and key that produces the highest score 
  while (getline(file, line)) {
    if (nlines == maxIndex){
      const char* input = line.c_str();
      strToHex(input, hexInput, strLen);
      for (uint i=0;i<hexLen;i++){
        cout << char(hexInput[i] ^ maxKey);
      }
      // cout << endl;
      break;
    }
    nlines++;
  }
  //


  // if (repRound == 0){
  //   /* We did not find anything with previous analysis.*/
  //   // Read file line by line
  //   ifstream file("4.txt");  

  //   // Scores and keys
  //   uint scores[nlines] = {0};
  //   uint newkeys[nlines] = {0};
  //   nlines = 0;

  //   while (getline(file, line)) {
  //     strLen = strlen(line.c_str());
  //     hexLen = strLen/2;
  //     const char* input = line.c_str();
      
  //     // Convert string input to hex
  //     strToHex(input, hexInput, strLen);

  //     // Idea
  //     fullScore(hexInput,hexLen,scores,newkeys,nlines);

  //     nlines++;
  //   }
  // }



  //

//   delete[] input;
  delete[] hexInput;
}