// Detect single-character XOR

// One of the 60-character strings in this file has been encrypted by single-character XOR.

// Find it.

// (Your code from #3 should help.)

#include <iostream>
#include <cstring>
#include <vector>
#include <bits/stdc++.h> //Vector functions
#include <fstream> // File handling
#include "functions.h"
using namespace std;

int main(){
  // Input variables
//   char *input = new char[BUFFER_SIZE+1];
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

  // Score
  vector<uint> scores;

  // Store scores
  vector<uint> scoreKeys;

  // What scored-based analysis finds
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

    // cout << "Length: " << strLen << endl;
    // cout << "Hex Lenght: " << hexLen << endl;

    // Convert string input to hex
    strToHex(input, hexInput, strLen);

    // Extract features of string: character count and consecutives characters.
    stringFeatures(hexInput, hexLen, repetitions, counters);

    // Analysis of the features, keys stores the possible keys.
    stringsAnalysis(hexInput, hexLen, repetitions, counters, repKeys, scores, scoreKeys);

    while(!repKeys.empty()){
        // uint finalKey = keys.back();
        finalKey = repKeys.back();
        repKeys.pop_back();
        correctness = true;
        for (uint i=0;i<hexLen;i++){
          // cout << char(hexInput[i] ^ finalKey);
          // cout << "H: " << hexInput[i] << ", key: " << finalKey << ". ";
          uint tmpVal = hexInput[i] ^ finalKey;
          if (!validChar(tmpVal)){
            // cout << tmpVal << endl;
            correctness = false;
            break;
          }
          if (correctness == false){
            break;
          }
        }
        // cout << endl;
        // cin.get();
    }

    if (correctness == true){
      repRound++;
    //   cout << "found sth: " << line.c_str() << endl;
      for (uint i=0;i<hexLen;i++){
        // cout << "H: " << hexInput[i] << ", key: " << finalKey << ". ";
        // cout << "(" << (hexInput[i] ^ finalKey) << ", " << char(hexInput[i] ^ finalKey) << "), ";
        cout << char(hexInput[i] ^ finalKey);
      }
      cout << endl;
      // cin.get();
    }
  }

  /* Repetition leads to nowhere, let's try scores*/
  if (repRound == 0){
    maxScore = *max_element(scores.begin(), scores.end());
    maxIndex = distance(scores.begin(),find(scores.begin(),scores.end(),maxScore));
    maxKey = scoreKeys[maxIndex];
    cout << maxScore << " is the max score at " << maxIndex << endl;
  }

  file.close();

  // Read file line by line
  //ifstream file("4.txt");
  file.clear();
  file.open("4.txt");

  nlines = 0;
  // Loop to print
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