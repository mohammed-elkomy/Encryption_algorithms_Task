//
// Created by Mohammed Alaa Elkomy on 05-Apr-18.
//

#include "Crypto.h"

Crypto::Crypto(string p_vocabulary) {
    //! construct a trivial random generator engine from a time-based seed:
    unsigned seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    gen = new default_random_engine(seed);

    vocabulary = move(p_vocabulary);
    dis = new uniform_int_distribution<>(0, static_cast<int>(vocabulary.length()));
}

Crypto::Crypto() : Crypto(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\n\t\b\r\a") {}

string Crypto::random_letter() {
    return string(1, vocabulary[(*dis)(*gen)]); //! get random letter
}

Crypto::~Crypto() { //! free ram
    delete (dis);
    delete (gen);
}

void Crypto::print_header(const string &text) {
    auto remaining = static_cast<int>(120 - text.size());

    for (int i = 0; i < remaining / 2; ++i)putchar('=');
    cout << text;
    for (int i = 0; i < remaining / 2; ++i)putchar('=');

    if (remaining % 2) putchar('=');
}

int Crypto::random_int() {
    return (*dis)(*gen);
}




