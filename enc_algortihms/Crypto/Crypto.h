//
// Created by Mohammed Alaa Elkomy on 4/5/2018.
//

#ifndef ENCRYPTION_ALGORITHMS_CRYPTO_H
#define ENCRYPTION_ALGORITHMS_CRYPTO_H

#include <string>
#include <random>
#include <utility>
#include <chrono>
#include <iostream>

using namespace std;
//! Crypto class.
/*!
  Basic implementation for cryptographic algorithm
*/
class Crypto {
protected:
    string vocabulary;
    string plainText;
    string cipherText;
    string key;

private:
    uniform_int_distribution<> *dis;
    default_random_engine *gen;

public:
    //! Crypto constructor.
    /*!
     \param vocabulary string (optional)
   */
    explicit Crypto(string p_vocabulary);

    Crypto();

    //! Crypto random_letter.
    /*!
        get random letter from vocab
   */
    string random_letter();
    int random_int();

    virtual //! Crypto destructor.
    ~Crypto();

    //! print header basic
    void print_header(const string &text);


    //! basic encrypt decrypt pair
    /*!
     \param text string
     \returns text string
   */
    virtual string encrypt(string pPlainText) = 0;
    virtual string decrypt() = 0;
};


#endif //ENCRYPTION_ALGORITHMS_CRYPTO_H
