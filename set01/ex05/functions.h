#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const int BUFFER_SIZE = 500;

// Key
const char key[] = {'I','C','E'};

void repKeyXOR(char* input, char* output, const char* key, uint size, uint keySize);

#endif