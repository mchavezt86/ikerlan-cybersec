// Void
// #include <iostream>
// #include <cstring>
// #include <sstream>
// #include <vector>
// #include <fstream>
// #include "functions.h"
// using namespace std;

#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const int BUFFER_SIZE = 100;
const int TOTAL_ASCII = 256;
const int REP_LETTERS_SIZE = 20;
const uint repLetters[REP_LETTERS_SIZE] = {'e','o','l','s','t','f','m','p','c','n','E', 'O', 'L', 'S', 'T', 'F', 'M', 'P', 'C', 'N'};
// const uint freqLetters[REP_LETTERS_SIZE] = {'e','a','r','i','o','t','n','s','l','c','E', 'A', 'R', 'I', 'O', 'T', 'N', 'S', 'L', 'C'};
const uint ranking[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 6, 13, 16, 25, 10, 8, 18, 21, 2, 4, 15, 12, 20, 22, 7, 1, 17, 19, 24, 14, 5, 11, 3, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void strToHex(const char* input, uint* result, uint strLen);

void stringFeatures(uint* hexInput, int hexLen, std::vector<uint>& rep, uint* counters);

void stringsAnalysis(uint* hexInput, uint hexLen, std::vector<uint> rep, uint* counters, std::vector<uint>& repKeys, std::vector<uint>& score, std::vector<uint>& scoreKeys);

bool validChar(char chr);

#endif