#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

const char input[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";

const unsigned char key[] = "YELLOW SUBMARINE";

const uint key_size = 16;

void ctr_mode(unsigned char* theKey);

#endif