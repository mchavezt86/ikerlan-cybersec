#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const uint key_size = 16;

void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex);

uint find_block_size(unsigned char* hexArray,uint valIndex, unsigned char* key);

void random_key(unsigned char* key);

void aes128ecb_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen);

void detect_block_cipher(unsigned char* input,uint outputSize, uint key_size);

char found_char(unsigned char* hexArray,uint valIndex, unsigned char* key, uint block_size);

#endif