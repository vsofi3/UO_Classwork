#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "cipher.h"
#include "Ccipher.h"

using namespace std;

void usage(int argc, char **argv);
void load_file(string &ct, char *fileName);
void check_cipher_text(string ct);

int main(int argc, char **argv)
{
    // Verify given arguments
    usage(argc, argv);

    // Load the cipher text from file
    string cipherText;
    load_file(cipherText, argv[1]);
    check_cipher_text(cipherText);

    // Load the Caesar Cipher value 
    int CaesarValue = 0;
    stringstream ss(argv[2]);
    ss >> CaesarValue;  

    // Get the string to encrypt
    string input;
    load_file(input, argv[3]);

    // Regular cipher encryption/decryption
    Cipher myCipher(cipherText);
    string output = myCipher.encrypt(input);
    cout << "Encrypted: " << output << endl;
    output = myCipher.decrypt(output);
    cout << "Decrypted: " << output << endl;

    // Caesar cipher encryption/decryption
    CaesarCipher myCCipher(cipherText, CaesarValue);
    output = myCCipher.encrypt(input);
    cout << "Encrypted: " << output << endl;
    output = myCCipher.decrypt(output);
    cout << "Decrypted: " << output << endl;

    return 0;
}

void usage(int argc, char **argv)
{
    if(argc < 4) {
        cout << "usage: " << argv[0] << 
                " <input file> <Caesar cipher value>"
                " <string to encrypt> "
             << endl;
        exit(EXIT_FAILURE);
    }
}

void load_file(string &in, char *fileName)
{
    ifstream inFile;
    inFile.open(fileName);
    string word;
    getline(inFile, in);
    inFile.close();
}


void check_cipher_text(string ct)
{
    if(ct.size() != 27) {
        cout << "Cipher text is not 27 characters long" << endl;
        exit(EXIT_FAILURE);
    }
}
