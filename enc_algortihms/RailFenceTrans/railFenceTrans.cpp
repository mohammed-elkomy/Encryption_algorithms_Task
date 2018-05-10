//
// Created by Mohammed Alaa Elkomy on 05-Apr-18.
//

#include "railFenceTrans.h"

//! railFenceTrans encrypt.
/*!
 * nice example : https://en.wikipedia.org/wiki/Rail_fence_cipher
 \param PlainText string
 \returns ciphertext string
*/
string railFenceTrans::encrypt(string pPlainText) {
    this->plainText = pPlainText;
    string cipherText(pPlainText);

    original_locations.clear();

    if (rail_fence_depth != 1) {
        //! build the key and cipher
        vector<vector<int>> temp_key(static_cast<unsigned int>(rail_fence_depth));
        vector<vector<char>> temp_cipher(static_cast<unsigned int>(rail_fence_depth));

        int fence_level = 0, delta = +1;
        for (int i = 0; i < pPlainText.size(); ++i, fence_level += delta) {

            if (fence_level == -1) //! can't go above
                fence_level = 1, delta *= -1; //! top corner

            else if (fence_level == rail_fence_depth) //! can't go deeper
                fence_level -= 2, delta *= -1;  //! bottom corner

            temp_cipher[fence_level].push_back(pPlainText[i]); //! push char to this level
            temp_key[fence_level].push_back(i); //! push key to this level
        }


        int c = 0;
        for (int i = 0; i < rail_fence_depth; ++i) //! for each level
            for (int j = 0; j < temp_key[i].size(); ++j, c++) //! for each char in that level
                cipherText[c] = temp_cipher[i][j], original_locations.push_back(temp_key[i][j]); //! the source index of each character

    } else
        for (int i = 0; i < plainText.size(); ++i)
            original_locations.push_back(i);

    this->cipherText = cipherText;
    return cipherText;
}

//! railFenceTrans decrypt.
/*!
 * decryption in rail fence is (n-1) transpositions but i will keep an index for efficient transposition
 * instead of o(n^2) it will be o(n) :)
 \param ciphertext string
 \returns PlainText string
*/
string railFenceTrans::decrypt() {
    string plainText(cipherText);

    if (rail_fence_depth != 1) {
        //!simply iterate on the key vector in o(n) instead of (n-1) transpositions !!
        for (int i = 0; i < original_locations.size(); ++i)
            plainText[original_locations[i]] = cipherText[i];
    }

    this->plainText = plainText;
    return plainText;
}

//! print last Cipher Text
void railFenceTrans::print_last_ciphertext_line() {
    print_header("Cipher Text");
    cout << cipherText << endl;
}

//! print last Plain Text
void railFenceTrans::print_last_plaintext_line() {
    print_header("Plain Text");
    cout << plainText << endl;
}

//! print last key transposition
void railFenceTrans::print_last_key() {
    print_header("Key");
    for (int i = 0; i < original_locations.size(); ++i)
        cout << original_locations[i] << ((i < original_locations.size() - 1) ? "," : "");
    cout << endl;
}