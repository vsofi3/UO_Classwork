#ifndef CIPHER_H_
#define CIPHER_H_
#include <string>
#include <iostream>

using namespace std;

class Cipher{
protected: 
    struct CipherCheshire;
    CipherCheshire *smile;
public:
    Cipher();
    Cipher(string in);
    virtual string encrypt(string raw);
    virtual string decrypt(string enc);
};

class CaesarCipher : public Cipher {
protected: 
    struct CaesarCipherCheshire;
    CaesarCipherCheshire *CaesarSmile;
public:
    CaesarCipher();
    CaesarCipher(string in, int rot);
    virtual string encrypt(string raw);
    virtual string decrypt(string enc);
};
#endif
