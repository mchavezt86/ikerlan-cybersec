#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

/*
Single-byte XOR cipher

The hex encoded string:

1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

... has been XOR'd against a single character. Find the key, decrypt the message.

You can do this by hand. But don't: write code to do it for you.

How? Devise some method for "scoring" a piece of English plaintext. Character frequency is a good metric. Evaluate each output and choose the one with the best score.
Achievement Unlocked

You now have our permission to make "ETAOIN SHRDLU" jokes on Twitter.
*/

const int BUFFER_SIZE = 100;
const int TOTAL_ASCII = 256;
const int REP_LETTERS_SIZE = 20;
uint repLetters[REP_LETTERS_SIZE] = {'e','o','l','s','t','f','m','p','c','n','E', 'O', 'L', 'S', 'T', 'F', 'M', 'P', 'C', 'N'};

/*
Note: printable ASCII range from 32 (space) to 126 (~), others are not part of English
*/

/* function to convert string to hex*/
void strToHex(char* input, uint* result, uint strLen){
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

    delete[] temp;
  }
}

/* function to find consecutives numbers in an array */
void findConsecutives(uint* hexInput, int hexLen, vector<uint>& rep){
  for (uint i=0;i<hexLen-1;i++){
    if (hexInput[i] == hexInput[i+1]){
      rep.push_back(i);
      // Test for repetition
      //cout << "Repetition at: " << i << endl;
    }
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
      cout << "Repetition at: " << i << endl;
    }
  }

  // Previous loop did not count the last one.
  counters[hexInput[hexLen-1]]++;
}

/* function to check if consecutives have information*/
void stringsAnalysis(uint* hexInput, uint hexLen,vector<uint> rep, uint* counters, vector<uint>& keys){
  uint max = 0, max_i;
  // uint possibleKeys[REP_LETTERS_SIZE];

  // Consecutives can be from repLetters
  while (!rep.empty()){
    uint index = rep.back();
    rep.pop_back();

    /*cout << "value: " << hexInput[index] << ", xor(e): " << (hexInput[index] ^ 101) << endl;
    cout << "value: " << hexInput[index] << ", xor(o): " << (hexInput[index] ^ 111) << endl;*/

    for (uint i=0;i<TOTAL_ASCII;i++){
    if (counters[i]>max){
      max = counters[i];
      max_i = i; //This stores the most repeated character
    }
  }

    for (uint i=0;i<REP_LETTERS_SIZE;i++){
      // possibleKeys[i] = hexInput[index] ^ repLetters[i];
      //cout << "most counter letter with key " << char(possibleKeys[i]) << ", is " << char(max_i ^ possibleKeys[i]) << "("<< (max_i ^ possibleKeys[i]) << ")" << endl;
      uint tmpKey = hexInput[index] ^ repLetters[i];
      uint tmpChar = max_i ^ tmpKey;

      if ((tmpChar == ' ') || ((tmpChar > 'A' && tmpChar < 'Z') || (tmpChar > 'a' && tmpChar < 'z'))){
        keys.push_back(tmpKey);
      }
    }
  }
}

int main(){
  // Input variables
  char *input = new char[BUFFER_SIZE+1];
  uint *hexInput = new uint[BUFFER_SIZE/2];

  // Array to count frequencies
  uint counters[TOTAL_ASCII] = {0};

  // Vector to store repetitions
  vector<uint> repetitions;

  // Length of varialbles
  uint strLen;
  uint hexLen;

  // Store possible keys
  vector<uint> keys;
  
  //Read input
  cout << "Enter hex encoded string: ";
  cin.getline(input, BUFFER_SIZE+1);

  strLen = strlen(input);
  hexLen = strLen/2;

  // Convert string input to hex
  strToHex(input, hexInput, strLen);

  // Printing values to test
  // cout << "hex values: ";
  // for (uint i=0;i<hexLen;i++){
  //   cout << hexInput[i] << ", ";
  // }
  // cout << endl;

  // Find consecutives chars
  // findConsecutives(hexInput, hexLen, repetitions);

  // Extract features of string: character count and consecutives characters.
  stringFeatures(hexInput, hexLen, repetitions, counters);

  // Analysis of the features, keys stores the possible keys.
  stringsAnalysis(hexInput, hexLen, repetitions, counters, keys);

  while(!keys.empty()){
    uint finalKey = keys.back();
    keys.pop_back();
    for (uint i=0;i<hexLen;i++){
      cout << char(hexInput[i] ^ finalKey);
    }
    cout << endl;
  }

  delete[] input;
  delete[] hexInput;
}