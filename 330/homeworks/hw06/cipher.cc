#include "cipher.h"

#define UPPER_CASE(r) ((r) - ('a' - 'A'))

struct Cipher::CipherCheshire {
    string cipherText;
};

Cipher::Cipher()
{
    smile = new CipherCheshire;
    smile->cipherText = "abcdefghijklmnopqrstuvwxyz ";
}
Cipher::Cipher(string in)
{
    smile = new CipherCheshire;
    smile->cipherText = in;
}
string Cipher::encrypt(string raw)
{
    string retStr;
    cout << "Encrypting..." << endl;
    for(unsigned int i = 0; i < raw.size(); i++) {
        unsigned int pos;
        bool upper = false;
        if(raw[i] == ' ') {
            pos = 26;
        } else if(raw[i] >= 'a') {
            pos = raw[i] - 'a';
        } else {
            pos = raw[i] - 'A';
            upper = 1;
        }
        if(upper) {
            retStr += UPPER_CASE(smile->cipherText[pos]);
        } else {
            retStr += smile->cipherText[pos];
        }
    }
    cout << "Done" << endl;

    return retStr;
}

string Cipher::decrypt(string enc)
{
    string retStr;
    cout << "Decrpyting..." << endl;
    // Fill in code here
    for(unsigned int i = 0; i < enc.size(); i++){
	unsigned int pos; 
	bool upper = false; 
	
	if(enc[i] == ' '){
		pos = 26;
	} else if (enc[i] >= 'a'){
		pos = enc[i] - 'a';
	} else {
		pos = enc[i] - 'A';
		upper = 1;
	}
	if (upper){
		retStr += UPPER_CASE(smile->cipherText[pos + 1]);
	} else {
		retStr += smile->cipherText[pos + 1];
	}
    }
    cout << "Done" << endl;

    return retStr;
}




struct CaesarCipher::CaesarCipherCheshire : CipherCheshire {
     int rot;
};

CaesarCipher::CaesarCipher()
{
    // Fill in code here
    CaesarSmile = new CaesarCipherCheshire; 
    CaesarSmile->cipherText = "abcdefghijklmnopqrstuvwxyz ";
    CaesarSmile->rot = 0; 
}

CaesarCipher::CaesarCipher(string in, int rot)
{
    // Fill in code here
    CaesarSmile = new CaesarCipherCheshire;
    CaesarSmile->cipherText = in;
    CaesarSmile->rot = rot;
}

string CaesarCipher::encrypt(string raw)
{
    string retStr;
    cout << "Encrypting..." << endl;
    // Fill in code here
	int rot = CaesarSmile->rot % 27; //handles case where rot is greater than 27     
	if (rot < 0) {
		rot = rot + 27;	// handles case when rot is negative
	}
	for(unsigned int i = 0; i < raw.size(); i++) {
	   unsigned int pos;
           bool upper = false;
           if(raw[i] == ' ') {
            	pos = 26 + rot; 
           } else if(raw[i] >= 'a') {
            	pos = (raw[i] - 'a') + rot;
       	   } else {
            	pos = (raw[i] - 'A') + rot;
           	upper = 1;
           }
       	   if (pos >= 27) {
		pos = pos%27; //if position went over 27, find appropriate index
	   }
	   if(upper) {
           	retStr += UPPER_CASE(CaesarSmile->cipherText[pos]);
       	   } else {
            	retStr += CaesarSmile->cipherText[pos];
       	   }
   	 
	}

    cout << "Done" << endl;

    return retStr;

}

string CaesarCipher::decrypt(string enc)
{
    string retStr;
    cout << "Decrpyting..." << endl;
    // Fill in code here

    
	int rot = CaesarSmile->rot % 27;     
	for(unsigned int i = 0; i < enc.size(); i++) {
	   unsigned int pos;
	   bool upper = false;
	  if(enc[i] == ' ') {
            	pos = 26 + 1 - rot;
           } else if(enc[i] >= 'a') {
            	pos = (enc[i] - 'a' + 1) - rot;
       	   } else {
            	pos = (enc[i] - 'A' + 1) - rot;
		upper = 1;
           }
	  if (pos > 27 && rot < 0){
		pos = (pos % 27);	//if rot is greater than 27, find appropriate index
	  }
	  if (pos > 27 && rot > 0){
		pos = (pos % 27) + 5 ;
		if (pos > 27) { 	
			pos = pos -  10;
		}
	  }
	  else if (pos < 0){
		pos = 27 + (pos % 27); //if position is negative
	  }
	  if(upper) {
		retStr += UPPER_CASE(CaesarSmile->cipherText[pos]);
       	   } else {
            	retStr += CaesarSmile->cipherText[pos];
       	   }
   	
	 }

    cout << "Done" << endl;

    return retStr;
}


