#include <iostream>
#include <string>
using namespace std;

/*
Convert hex to base64

The string:

49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

Should produce:

SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t

So go ahead and make that happen. You'll need to use this code for the rest of the exercises.

Cryptopals Rule:
Always operate on raw bytes, never on encoded strings. Only use hex and base64 for pretty-printing.
*/

char toBase64(uint dig){
    char chrDig;
    if (dig < 26){
      chrDig = int('A') + dig;
    } else if (dig < 52){
      chrDig = int('a') + dig - 26;
    } else if (dig < 62 ){
      chrDig = int('0') + dig - 52;
    } else if (dig < 63 ){
      chrDig = 43;
    } else{
      chrDig = 47;
    }
    return chrDig;
}

void convert(const string s, int l, string & output){
  uint number = stoul(s,0,16);
//   cout << to_string(number) << "\n";
  uint dig1 = number >> 6;
  uint dig2 = number % 64;
//   cout << to_string(dig1) << to_string(dig2);
//   cout << toBase64(dig1) << toBase64(dig2);
//   output.append(to_string(toBase64(dig1)));
//   output.append(to_string(toBase64(dig2)));
  output += toBase64(dig1);
  output += toBase64(dig2);
}

int main() {
  //Variables
  string hexNumber;
  string base64Number = "";
  uint strLen;

  //Input
  cout << "Type the hex number to convert: ";
  getline (cin, hexNumber);
  
  //Get lenght of string
  //cout << "Lenght is :" << hexLen << "\n";
  uint hexLen = hexNumber.length();

  //Divide full string into segments of 3
  for (uint i=0;i<hexLen;i+=3){
    uint8_t min = hexLen - i;
    if (min < 3){
      //cout << "i: " << hexNumber.substr(i,hexLen - i) << "\n";
      convert(hexNumber.substr(i,min),min,base64Number);
      //Here there should be a handle for when having less than 3 digits.
    } else{
      //cout << "i: " << hexNumber.substr(i,3) << "\n";
      convert(hexNumber.substr(i,3),3,base64Number);
    }
  }

  cout << "The base64 number is: " << base64Number << "\n";
  return 0;
}