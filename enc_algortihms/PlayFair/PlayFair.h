//
// Created by Mohammed Alaa Elkomy on 4/5/2018.
//
#include <iostream>
#include <set>
#include <map>
#include "../Crypto/Crypto.h"

#ifndef ENCRYPTION_ALGORITHMS_PLAYFAIR_H
#define ENCRYPTION_ALGORITHMS_PLAYFAIR_H


class PlayFair : Crypto {

protected:
    //! conversion indexed data structures
    map<char, pair<int, int>> char2position; //! char indexed
    vector<vector<char>> position2char; //! 2d grid

public:
    PlayFair();
    explicit PlayFair(const string &p_vocabulary);

private:
    //! balanceCost
    /*!
     * finds the cost of a key matrix >> more squared = less cost
     \param width int
     \param height int
   */
    double balanceCost(int h, int w) { return (1.0 * h / w - 1) * (1.0 * h / w - 1); }
    string preProcessPlaintext(string plaintext);


public:
    //! build char matrix used for playfair
    void BuildMatrixFromKey(string key_string);

    //! playfair encrypt,decrypt pair
    string encrypt(string pPlainText) override;
    string decrypt() override;

    //! prints
    void print_last_ciphertext() ;
    void print_last_plaintext();
    void print_last_key() ;

    //! print vocabulary basic
    void print_vocabulary();

};


#endif //ENCRYPTION_ALGORITHMS_PLAYFAIR_H


