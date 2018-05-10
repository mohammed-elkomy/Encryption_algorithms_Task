//
// Created by Mohammed Alaa Elkomy on 09-May-18.
//

#include "Keyless.h"


string Keyless::decrypt() {
    return encrypt(this->cipherText);
}

void Keyless::print_last_ciphertext() {
    print_header("Cipher Text");
    cout << cipherText << endl;
}

void Keyless::print_last_plaintext() {
    print_header("Plain Text");
    cout << plainText << endl;
}

string Keyless::encrypt(string pPlainText) {
    this->plainText = pPlainText;
    string cipherText(pPlainText);

    original_locations.clear();

    char temp;
    //! reverse
    for (int i = 0; i < cipherText.size() / 2; i++) {
        temp = cipherText[i];
        cipherText[i] = cipherText[cipherText.size() - 1 - i];
        cipherText[cipherText.size() - 1 - i] = temp;
    }

    this->cipherText = cipherText;
    return cipherText;
}