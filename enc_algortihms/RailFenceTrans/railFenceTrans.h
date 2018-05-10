//
// Created by Mohammed Alaa Elkomy on 05-Apr-18.
//

#ifndef ENCRYPTION_ALGORITHMS_TRANSPOTION_H
#define ENCRYPTION_ALGORITHMS_TRANSPOTION_H


#include "../Crypto/Crypto.h"

class railFenceTrans : Crypto {

private:
    int rail_fence_depth;
    vector<int> original_locations;
public:
    //! railFenceTrans constructor.
    /*!
     \param depth int
   */
    explicit railFenceTrans(int Pdepth) { rail_fence_depth = Pdepth; }

    //! railFenceTrans .
    /*!
     \param depth int
   */
    string encrypt(string pPlainText) override;
    string decrypt() override;

    //! prints
    void print_last_ciphertext_line();
    void print_last_plaintext_line();
    void print_last_key();
};


#endif //ENCRYPTION_ALGORITHMS_TRANSPOTION_H
