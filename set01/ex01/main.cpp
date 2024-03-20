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

/* Logic here is to divide the input into segments of 3 due to the representation:
   - 3 octect (each octect is a hex digi) equal two base64 digits. 
   - Special cases: padding is required when having 1 or 2 octects left
   - The input is trimmed to match an even number of digits, if odd the last 
   octect is ignored*/

#include <iostream>
#include <string>
using namespace std;

/* function toBase64:
   - input: uint digit
   - output: corresponding Base64 digit.
   Receives a 3-char string, convert it to number, computes the corresponding
   2 octects and appends them to the string (output)*/
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

/* function convert:
   - input: 3-char string, output string
   Receives a 3-char string, convert it to number, computes the corresponding
   2 octects and appends them to the string (output)*/
void convert(const string s, string & output){
  uint number = stoul(s,0,16);
  uint dig1 = number >> 6;
  uint dig2 = number & 0x3F;
  output += toBase64(dig1);
  output += toBase64(dig2);
}

/* function convert_1:
   - input: 1-char string, output string
   Receives a 1-char string, convert it to number, computes the corresponding
   octect and appends it to the string (output). It also append one '=' padding*/
void convert_1(const string s, string & output){
  uint number = stoul(s,0,16);
  uint dig1 = number << 2;
  output += toBase64(dig1);
  output += '=';
}

/* function convert_2:
   - input: 2-char string, output string
   Receives a 2-char string, convert it to number, computes the corresponding
   2 octects and appends them to the string (output). It also append two '=' padding*/
void convert_2(const string s, string & output){
  uint number = stoul(s,0,16);
  uint dig1 = number >> 2;
  uint dig2 = number & 0x03;
  dig2 = dig2 << 4;
  output += toBase64(dig1);
  output += toBase64(dig2);
  output += '=';
  output += '=';
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
  uint hexLen = hexNumber.length();
  if (hexLen%2 != 0){
    hexLen--;
  }

  //Divide full string into segments of 3
  for (uint i=0;i<hexLen;i+=3){
    uint8_t min = hexLen - i;
    if (min < 3){
      /* Here there should be a handle for when having less than 3 digits.
         with special functions which also handle padding withg '=' */
      cout << min;
      if (min == 1){
        convert_1(hexNumber.substr(i,min),base64Number);
      } else if (min == 2){
        convert_2(hexNumber.substr(i,min),base64Number);
      }
    } else{
      //cout << "i: " << hexNumber.substr(i,3) << "\n";
      convert(hexNumber.substr(i,3),base64Number);
    }
  }

  cout << "The base64 number is: " << base64Number << "\n";
  return 0;
}