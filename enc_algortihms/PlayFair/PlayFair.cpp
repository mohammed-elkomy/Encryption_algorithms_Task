//
// Created by Mohammed Alaa Elkomy on 05-Apr-18.
//

#include "PlayFair.h"
#include <stdexcept>
#include <algorithm>

PlayFair::PlayFair() : Crypto::Crypto() {}

PlayFair::PlayFair(const string &p_vocabulary) : Crypto(p_vocabulary) {
    vocabulary += "_"; //! special character for used for padding
}

void PlayFair::print_last_ciphertext() {
    print_header("Cipher Text");
    cout << cipherText << endl;
}

void PlayFair::print_last_plaintext() {
    print_header("Plain Text");
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '_'), plainText.end()); //! remove any special character i.e. _
    cout << plainText << endl;
}


void PlayFair::print_last_key() {
    print_header("Key");
    for (vector<char> row:position2char) { //! indexed by position
        for (char col_char:row)
            switch (col_char) {
                case '\n':
                    cout << "\\n";
                    break;
                case '\t':
                    cout << "\\t";
                    break;
                case '\b':
                    cout << "\\b";
                    break;
                case '\r':
                    cout << "\\r";
                    break;
                case '\a':
                    cout << "\\a";
                    break;
                default:
                    putchar(col_char);
            }

        cout << endl;
    }
}

string PlayFair::preProcessPlaintext(string plaintext) {
    for (int i = 1; i < plaintext.length(); i += 2) { //!inject a special character if both characters are equal so can't be indexed from matrix
        if (plaintext[i] == plaintext[i - 1]) {
            plaintext.insert(static_cast<unsigned int>(i), "_");//! we need a special character to guarantee the integrity of the message
        }
    }
    return plaintext;
}


void PlayFair::BuildMatrixFromKey(string key_string = "PLAYFAIR") {
    this->key = key_string;//!key text

    set<char> key_set(key_string.begin(), key_string.end());  //! make sure there is no duplications
    string key_bag; //!key set but ordered as entered

    for (auto original :key_string) { //! ordered set of keys as entered
        if (key_set.find(original) != key_set.end()) {
            key_bag.push_back(original); //! push it in order
            key_set.erase(original); //! remove from key char set
        }
    }

    char2position.clear();
    position2char.clear();

    //! check the cost of the matrix
    //! try to be squared as possible
    auto height_candidate = static_cast<int>(sqrt(vocabulary.length()));
    auto height_candidate_ = height_candidate;

    //! extend one candidate and shrink the other one
    while (vocabulary.length() % height_candidate) height_candidate++;
    while (vocabulary.length() % height_candidate_)height_candidate_--;

    //! compute width .. must be divisible
    auto wid_candidate = static_cast<int> (vocabulary.length() / height_candidate), wid_candidate_ = static_cast<int>(vocabulary.length() / height_candidate_);

    //! final
    int wid, hei;

    //! pick the least cost
    if (balanceCost(height_candidate, wid_candidate) < balanceCost(height_candidate_, wid_candidate_))
        wid = wid_candidate, hei = height_candidate;
    else
        wid = wid_candidate_, hei = height_candidate_;

    //! key position to index row
    vector<char> row(static_cast<unsigned int>(wid));

    //! make the height * wid vector
    for (int l = 0; l < hei; ++l)
        position2char.push_back(row);

    //!remaining in vocabulary
    set<char> remaining(vocabulary.begin(), vocabulary.end());

    //! remove the characters used in the key for example "PLAYFAIR"
    for (char key_char: key_bag)
        remaining.erase(key_char); //! remove key charachter from vocabulary

    for (int i = 0, k = 0, v = 0; i < hei; ++i)
        for (int j = 0; j < wid; ++j)
            if (k < key_bag.size()) //! first k element form the key bag
                position2char[i][j] = key_bag[k++];
            else {
                //! after putting the key .. put the remaining in alphabetical order

                while (remaining.find(vocabulary[v++]) == remaining.end()); //! next char from vocabulary (not yet used) (found in remaining)

                position2char[i][j] = vocabulary[v - 1], remaining.erase(vocabulary[v - 1]); //! put it and remove it from the remaining
            }


    //!build inverted index
    for (int i = 0; i < hei; ++i)
        for (int j = 0; j < wid; ++j)
            char2position[position2char[i][j]].first = i, char2position[position2char[i][j]].second = j;


}


string PlayFair::encrypt(string pPlainText) {
    this->plainText = pPlainText;

    set<char> vocab_set(vocabulary.begin(), vocabulary.end());

    for (auto plain_char:pPlainText)
        if (vocab_set.find(plain_char) == vocab_set.end()) {
            cipherText = plainText = "Some letter in the plain text is not contained in vocabulary";
            return "";
        }

    if (position2char.empty())
        BuildMatrixFromKey(key);

    pPlainText = preProcessPlaintext(pPlainText); //! handel consecutive characters
    if (pPlainText.length() % 2) pPlainText += "_";//! make sure it is even length

    //! encrypt logic
    string ciphertext(pPlainText);

    for (int i = 0; i < pPlainText.length(); i += 2) { //! process each pair
        if (char2position[pPlainText[i]].first == char2position[pPlainText[i + 1]].first)//! same row >> go right and wrap around
        {
            ciphertext[i] = position2char[char2position[pPlainText[i]].first][(char2position[pPlainText[i]].second + 1) % position2char[0].size()]; //! first char in the pair
            ciphertext[i + 1] = position2char[char2position[pPlainText[i + 1]].first][(char2position[pPlainText[i + 1]].second + 1) % position2char[0].size()]; //! second char in the pair

        } else if (char2position[pPlainText[i]].second == char2position[pPlainText[i + 1]].second) //! same column >> go down and wrap around
        {
            ciphertext[i] = position2char[(char2position[pPlainText[i]].first + 1) % position2char.size()][char2position[pPlainText[i]].second]; //! first char in the pair
            ciphertext[i + 1] = position2char[(char2position[pPlainText[i + 1]].first + 1) % position2char.size()][char2position[pPlainText[i + 1]].second]; //! second char in the pair
        } else { //! keep row but replace column
            ciphertext[i] = position2char[char2position[pPlainText[i]].first][char2position[pPlainText[i + 1]].second]; //! first char in the pair
            ciphertext[i + 1] = position2char[char2position[pPlainText[i + 1]].first][char2position[pPlainText[i]].second]; //! second char in the pair
        }
    }

    this->cipherText = ciphertext;
    return ciphertext;
}


string PlayFair::decrypt() {
    if (position2char.empty())
        BuildMatrixFromKey(key);

    if (cipherText.length() % 2) {
        cipherText = "INVALID CIPHER:has odd length";
        return "";
    }

    set<char> vocab_set(vocabulary.begin(), vocabulary.end());

    for (auto plain_char:cipherText)
        if (vocab_set.find(plain_char) == vocab_set.end()) {
            cipherText = "Some letter in the cipher text is not contained in vocabulary";
            return "";
        }

    //! decrypt logic
    string plainText(cipherText);
    for (int i = 0; i < cipherText.length(); i += 2) {

        if (char2position[cipherText[i]].first == char2position[cipherText[i + 1]].first)//! same row >> go left and wrap around
        {
            plainText[i] = position2char[char2position[cipherText[i]].first][(char2position[cipherText[i]].second - 1 + position2char[0].size()) % position2char[0].size()]; //! first char in the pair
            plainText[i + 1] = position2char[char2position[cipherText[i + 1]].first][(char2position[cipherText[i + 1]].second - 1 + position2char[0].size()) % position2char[0].size()]; //! second char in the pair

        } else if (char2position[cipherText[i]].second == char2position[cipherText[i + 1]].second) //! same column >> go up and wrap around
        {
            plainText[i] = position2char[(char2position[cipherText[i]].first - 1 + position2char.size()) % position2char.size()][char2position[cipherText[i]].second]; //! first char in the pair
            plainText[i + 1] = position2char[(char2position[cipherText[i + 1]].first - 1 + position2char.size()) % position2char.size()][char2position[cipherText[i + 1]].second]; //! second char in the pair
        } else { //!keep row but exchange column same as encrypt
            plainText[i] = position2char[char2position[cipherText[i]].first][char2position[cipherText[i + 1]].second]; //! first char in the pair
            plainText[i + 1] = position2char[char2position[cipherText[i + 1]].first][char2position[cipherText[i]].second]; //! second char in the pair
        }
    }

    this->plainText = plainText;
    return plainText;
}

void PlayFair::print_vocabulary() {
    print_header("Vocabulary");
    cout << vocabulary << endl;
}

