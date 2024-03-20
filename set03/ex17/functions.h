#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const uint key_size = 16;

const uint max_size = 400;

void fullAES(unsigned char* output, unsigned char* key, unsigned char* iv, uint* full_size);

bool padding_check(unsigned char* output,unsigned char* key, unsigned char* iv, uint full_size);

#endif