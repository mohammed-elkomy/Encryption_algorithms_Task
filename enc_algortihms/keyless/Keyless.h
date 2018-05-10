//
// Created by Mohammed Alaa Elkomy on 09-May-18.
//

#ifndef ENCRYPTION_ALGORITHMS_KEYLESS_H
#define ENCRYPTION_ALGORITHMS_KEYLESS_H


#include "../Crypto/Crypto.h"

class Keyless : Crypto {
    vector<int> original_locations;

public:
    string encrypt(string pPlainText) override;
    string decrypt() override;

    //! prints
    void print_last_ciphertext();
    void print_last_plaintext();
};


#endif //ENCRYPTION_ALGORITHMS_KEYLESS_H
