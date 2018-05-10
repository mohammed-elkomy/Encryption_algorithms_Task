//
// Created by Mohammed Alaa Elkomy on 30-Apr-18.
//

#ifndef ENCRYPTION_ALGORITHMS_CONVERSION_H
#define ENCRYPTION_ALGORITHMS_CONVERSION_H


#include <map>
#include <utility> #include "BigInt.h"

class Conversion {
private:
    string indexToChar{};
    map<char, long long> charToIndex;


public:
    explicit Conversion(string);

    void setVocab(string);

    string convert_to_voacb_base(BigInt);

    BigInt convert_to_num_base(string);
};

string Conversion::convert_to_voacb_base(BigInt num_decimal_base) {
    stringstream ss;
    long long base = indexToChar.size();
    //! div and mod by base
  /*  while (num_decimal_base > BigInt(0)) {
        ss << indexToChar[num_decimal_base % base];
        num_decimal_base /= base;
    }*/

    for (; num_decimal_base > BigInt(0); num_decimal_base /= base)
        ss << indexToChar[num_decimal_base % base]; //! pushing least significant first which is obtained by mod

    string result = ss.str();
    char temp;
    //! reverse
    for (int i = 0; i < result.size() / 2; i++) {
        temp = result[i];
        result[i] = result[result.size() - 1 - i];
        result[result.size() - 1 - i] = temp;
    }
    return result;
}

BigInt Conversion::convert_to_num_base(string num_vocab_base) {
    BigInt conv;
    long long base = indexToChar.size();

    //! to decimal space
    for (char ch : num_vocab_base)  //! start from most significant digit
        conv = conv * base + BigInt(charToIndex[ch]);

    return conv;
}

void Conversion::setVocab(string vocab) {
    this->indexToChar = vocab;
    for (int i = 0; i < vocab.size(); i++) {
        charToIndex[vocab[i]] = static_cast<char>(i);
    }
}

Conversion::Conversion(string vocab) {
    setVocab(std::move(vocab));
}

#endif //ENCRYPTION_ALGORITHMS_CONVERSION_H
