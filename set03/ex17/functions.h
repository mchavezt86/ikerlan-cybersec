#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const uint key_size = 16;

const uint max_size = 400;

// void decodeBase64(const char* input, uint strLen, unsigned char* hexArray, uint* valIndex);

// uint padding(unsigned char* input, uint strLen, uint blockSize);

// unsigned char random_byte();

// void random_key(unsigned char* key);

// void aes128cbc_encrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const char* iv);

// void aes128cbc_decrypt(unsigned char* hexArray, unsigned char* output, uint hexSize, const unsigned char* theKey, uint keyLen, const char* iv);

void fullAES(unsigned char* output, unsigned char* key, unsigned char* iv, uint* full_size);

bool padding_check(unsigned char* output,unsigned char* key, unsigned char* iv, uint full_size);

#endif