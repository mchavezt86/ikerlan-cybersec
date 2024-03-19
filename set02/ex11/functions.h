#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const uint key_size = 16;

void aes128ecb_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen);

void aes128cbc_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, unsigned char* iv);

void random_key(unsigned char* key);

void encryption_oracle(unsigned char* hexInput,unsigned char* hexOutput, uint inputSize, uint* outputSize);

#endif