//
// Created by Mohammed Alaa Elkomy on 30-Apr-18.
//

#ifndef ENCRYPTION_ALGORITHMS_RSA_H
#define ENCRYPTION_ALGORITHMS_RSA_H


#include <utility>

#include "../Crypto/Crypto.h"
#include "../../UTIL/BigInt.h"

class RSA : Crypto {
private:
    int bit_size;
    BigInt p, q, n, phi, e, d, plain_BigInt, cipher_BigInt;
    Conversion *conversion;

public:
    //! RSA constructor.
    /*!
     \param bit_size int
   */
    explicit RSA(int bit_size, string vocabulary);

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

    void print_all_values();

    void print_vocabulary();



    ~RSA() {
        delete conversion;
    }
};

//
// Created by Mohammed Alaa Elkomy on 30-Apr-18.
//

#include <stdexcept>
#include <set>

void RSA::print_last_plaintext_line() {
    print_header("Plain Text");
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '_'), plainText.end()); //! remove any special character
    cout << "Text: " << plainText << endl;

    if (!plain_BigInt.isZero())
        cout << "Num: " << plain_BigInt << endl;
}

string RSA::encrypt(string pPlainText) {
    set<char> vocab_set(vocabulary.begin(), vocabulary.end());
    plainText = "_" + pPlainText; //! some character may be zero

    for (auto plain_char:plainText)
        if (vocab_set.find(plain_char) == vocab_set.end()) {
            cipherText = plainText = "Some letter in the plain text is not contained in vocabulary";
            plain_BigInt = 0;
            return "";
        }
    BigInt message = conversion->convert_to_num_base(plainText);

    plain_BigInt = message;

    if (message >= n) {
        cipherText = plainText = "The message is to large to fit into n space";
        plain_BigInt = 0;
        return "";
    }

    BigInt cipher = message.powMod(e, n);
    cipher_BigInt = cipher;
    cipherText = conversion->convert_to_voacb_base(cipher);
    return cipherText;
}

string RSA::decrypt() {
    set<char> vocab_set(vocabulary.begin(), vocabulary.end());

    for (auto cipher_char:cipherText)
        if (vocab_set.find(cipher_char) == vocab_set.end()) {
            cipherText = "Some letter in the cipher text is not contained in vocabulary";
            cipher_BigInt = 0;
            return "";
        }
    BigInt cipher = conversion->convert_to_num_base(cipherText);

    return conversion->convert_to_voacb_base(cipher.powMod(d, n));
}


void RSA::print_last_ciphertext_line() {
    print_header("Cipher Text");
    cout << "Text: " << cipherText << endl;
    if (!cipher_BigInt.isZero())
        cout << "Num: " << cipher_BigInt << endl;

}

void RSA::print_last_key() {
    print_header("Public Key Pair");
    cout << "e: " << e << endl << "n: " << n << endl;

    print_header("Private Key Pair");
    cout << "d: " << d << endl << "n: " << n << endl;

    print_header("(E*D)%Phi");
    cout << "Euler Identity: (E*D)%Phi=" << (e * d) % phi << endl;
}

RSA::RSA(int bit_size, string vocabulary) {
    this->bit_size = bit_size;
    this->vocabulary = "_" + std::move(vocabulary); //! zero character

    conversion = new Conversion(this->vocabulary);

    do {
        p = random_bigPrime(bit_size);
        q = random_bigPrime(bit_size);
        n = p * q;
        phi = (p - BigInt(1)) * (q - BigInt(1));
        e = primes[primes.size() / 2 + (random_int() % (primes.size() / 2))]; //! public key is random prime

        d = multiplicative_inverse(e, phi);
    } while (d == BigInt(0));
}

void RSA::print_all_values() {
    print_header("Internal Values");
    cout << "p: " << p << endl << "q: " << q << endl << "n: " << n << endl << "phi: " << phi << endl << "e: " << e << endl << "d: " << d << endl;
}

void RSA::print_vocabulary() {
    print_header("Vocabulary");
    cout << vocabulary << endl;

}

#endif //ENCRYPTION_ALGORITHMS_RSA_H
