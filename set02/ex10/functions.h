#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const unsigned char theKey[] = "YELLOW SUBMARINE";
const unsigned char iv[] = "0000000000000000";
const uint keyLen = 16;

void padding(unsigned char* input, uint strLen, uint blockSize);

void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex);

void aes128cbc_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const unsigned char* iv);

void aes128cbc_decrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const unsigned char* iv);

#endif