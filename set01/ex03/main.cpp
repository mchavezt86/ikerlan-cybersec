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

/* [The logic of this code is flawed but is corrected in for the next exercise.]
   This particular string has a particular feature: it has consecutive characters. Consecutive characters are not
   very common in English with a few exceptions. The repLetters array contains the letters with the possible
   consecutive characters, such as "ee" - seen, "oo"- foot, "tt" - attain, 'f' - affair and so on. Using this
   we retrieve a set of possible keys.
   To verify, we count the frequency of the encoded characater and decode the most frequent encoded character. Then
   we decode it using the possible keys and if the most repeated character is part of the letters from the 
   alphabet, we found the key */

/* function strToHex: convert string to hex
   - input: char pointer to input, uint pointer to results, uint length of the input
   - output: stored in result
*/
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

/* function stringFeatures: function to find features of the string
   - input: uint pointer to input in numerical version, uint length of the input, vector to store repetitinos
      and uint pointer to store the frequency of a particular encoded character.
   - output: stored in rep vector and counters pointer.
  Attempts to find the consecutive characters and stores is in a vector while saving the frequency of the 
  encoded characters.
*/
void stringFeatures(uint* hexInput, int hexLen, vector<uint>& rep, uint* counters){
  for (uint i=0;i<hexLen-1;i++){
    // counters index is the hex value and the content of the array is the frequency.
    counters[hexInput[i+1]]++;

    // Find consecutives and push it to vector.
    if (hexInput[i] == hexInput[i+1]){
      rep.push_back(i);
      // Test for
      cout << "Repetition at: " << i << endl;
    }
  }

  // Previous loop did not count the last one.
  counters[hexInput[hexLen-1]]++;
}

/* function stringsAnalysis: function to check if consecutives have information
   - input: uint pointer to input in numerical version, uint length of the input, vector with indexes of 
    repetitions, uint pointer with the frequency the encoded characters.
   - output: stored in key vector, with possible keys.
  Using the found consecutive characters stored in the repetitions vector, the code iterates these encoded
  characters and XORes against the array with characters that of the consecutive characters that appear 
  most in English to recover possible keys. Then compares the most repeated encoded character in the encoded
  string to an array that stores the most repeated characters in English. If the decoded most repeated 
  character is part of the alphabet or is a space, the key is correct.
*/
void stringsAnalysis(uint* hexInput, uint hexLen,vector<uint> rep, uint* counters, vector<uint>& keys){
  uint max = 0, max_i;

  // Consecutives can be from repLetters
  while (!rep.empty()){
    uint index = rep.back();
    rep.pop_back();

    /* Loop to find the most repeated encoded character, the index is the numerical value of this character*/
    for (uint i=0;i<TOTAL_ASCII;i++){
      if (counters[i]>max){
        max = counters[i];
        max_i = i; //This stores the most repeated character
      }
  }
    /* Loop the letters that are most likely to be repeated in English and XOR it agains the found repeated
       charactesr, to get the potential key. */
    for (uint i=0;i<REP_LETTERS_SIZE;i++){
      /* Computed possible key from the repetitions in English and the repetition found in the input*/
      uint tmpKey = hexInput[index] ^ repLetters[i];
      /* tmpChar stores the possible decoded most repeated character, decoded using the computed key */
      uint tmpChar = max_i ^ tmpKey;

      /* If the most frequent character, decoded with the possible key, is part of the alphabet or is a
        spaces we store the key */
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

  // Extract features of string: character count and consecutives characters.
  stringFeatures(hexInput, hexLen, repetitions, counters);

  // Analysis of the features, keys stores the possible keys.
  stringsAnalysis(hexInput, hexLen, repetitions, counters, keys);

  // Using the most likely key we decode the whole message.
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