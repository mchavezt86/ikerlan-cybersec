#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const uint key_size = 16;

void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex);

uint padding(unsigned char* input, uint strLen, uint blockSize);

unsigned char random_byte();

void random_key(unsigned char* key);

#endif