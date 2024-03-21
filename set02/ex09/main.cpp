#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;

// Implement PKCS#7 padding

// A block cipher transforms a fixed-sized block (usually 8 or 16 bytes) of plaintext into ciphertext. 
// But we almost never want to transform a single block; we encrypt irregularly-sized messages.

// One way we account for irregularly-sized messages is by padding, creating a plaintext that is an even 
// multiple of the blocksize. The most popular padding scheme is called PKCS#7.

// So: pad any block to a specific block length, by appending the number of bytes of padding to the end
// of the block. For instance,

// "YELLOW SUBMARINE"

// ... padded to 20 bytes would be:

// "YELLOW SUBMARINE\x04\x04\x04\x04"

/* Direct solution:
  Compute the size of the padding accounting these options:
  1) The block size is larger than the message -> padding lenght is just a substraction
  2) The message is larger than the block size -> we need to divide and the remainder is the padding length
  3) If the message lenght is a multiple of the block size, add a full block as padding. */

const int BUFFER_SIZE = 100;

/* function padding: Adds padding to an array.
   - input: char * to the input, uint for the input lenght and uint for block size.
   - output: uint with the final lenght, including padding.
*/
uint padding(char* input, uint strLen, uint blockSize){
//   cout << blockSize << "," << strLen << endl;
  uint pad;
  
  if (blockSize / strLen > 0){
    pad = blockSize - strLen;
  } else{
    // uint div = (strLen / blockSize);
    // pad = blockSize - (strLen - (div * blockSize));
    pad = strLen % blockSize;
    cout << pad << endl;
  }
  if (pad == 0){
    pad = blockSize;
  }

  for (uint i=0;i<pad;i++){
    input[strLen+i] = pad;
    // cout << pad << endl;
  }
  return strLen + pad;
}

int main(){
  // Input variable
  char *input = new char[BUFFER_SIZE+1];
  uint strLen,strLen_pad,blockSize,nblocks;
  string blockStr;

  //Read input
  cout << "Enter hex encoded string: ";
  cin.getline(input, BUFFER_SIZE+1);
  
  strLen = strlen(input);

  // Get block length
  cout << "Enter the block number: ";
  getline(cin,blockStr);

  // Convert to int
  blockSize = stoi(blockStr);

  // Apply padding to original input
  strLen_pad = padding(input,strLen,blockSize);
  // strLen = strLen / blockSize + 1;

  for (uint i=0;i<strLen_pad;i++){
    if (i<strLen){
      cout << input[i];
    } else{
      cout << "\\x" << hex << setw(2) << setfill('0') << uint(input[i]);
    }
    
  }
  cout << endl;

  return 0;
}