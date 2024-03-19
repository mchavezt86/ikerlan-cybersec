#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;


/*
Fixed XOR

Write a function that takes two equal-length buffers and produces their XOR combination.

If your function works properly, then when you feed it the string:

1c0111001f010100061a024b53535009181c

... after hex decoding, and when XOR'd against:

686974207468652062756c6c277320657965

... should produce:

746865206b696420646f6e277420706c6179
*/

const int BUFFER_SIZE = 36;

void xor_buffers(char* b1, char* b2, char* result, uint buffer_size){
  for (uint i=0;i<buffer_size;i+=2){
    //Variables to traverse the array
    char *temp1 = new char[2];
    char *temp2 = new char[2];
    stringstream ss1, ss2, ssxor;
    uint hex1, hex2, hexXor;

    // Array traverse 2 by 2
    memcpy(temp1,b1+i,2);
    memcpy(temp2,b2+i,2);

    // Convert to hex using string stream
    ss1 << hex << temp1;
    ss2 << hex << temp2;

    ss1 >> hex1;
    ss2 >> hex2;

    hexXor = hex1 ^ hex2;

    ssxor << hex << hexXor;

    // cout << ssxor.str().c_str();
    memcpy(result+i,ssxor.str().c_str(),2);

    delete[] temp1;
    delete[] temp2;
  }
}

int main(){
  
  char *buffer1 = new char[BUFFER_SIZE+1];
  char *buffer2 = new char[BUFFER_SIZE+1];
  char *result = new char[BUFFER_SIZE];

  //Read buffers
  cout << "Enter first buffer: ";
  cin.getline(buffer1, BUFFER_SIZE+1);
  cout << "Enter second buffer: ";
  cin.getline(buffer2, BUFFER_SIZE+1);

  xor_buffers(buffer1,buffer2,result,BUFFER_SIZE);
  cout << "Output buffer: " << result << endl;
}