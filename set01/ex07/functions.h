#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const unsigned char theKey[] = "YELLOW SUBMARINE";
const uint keyLen = 16;

void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex);

void aes128ecb(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen);

#endif