/*
#include<iostream>


using namespace std;



int main() {
    int x = 0;
    int y = 0;
    cout << extended_gcd(1180, 482, x, y);
    cout <<" "<< x <<" "<< y;
}




*/


#include <iostream>
#include <algorithm>
#include "enc_algortihms/RailFenceTrans/railFenceTrans.h"
#include "enc_algortihms/PlayFair/PlayFair.h"
#include "UTIL/BigInt.h"
#include "UTIL/conversion.h"
#include "enc_algortihms/RSA/RSA.h"
#include "enc_algortihms/keyless/Keyless.h"


using namespace std;


void print_header(const string &text) {
    auto remaining = static_cast<int>(120 - text.size());
    for (int i = 0; i < remaining / 2; ++i)putchar('=');
    cout << text;
    for (int i = 0; i < remaining / 2; ++i)putchar('=');
    if (remaining % 2) putchar('=');
}


void run_play_fair_demo() {
    print_header("Welcome To PlayFair");

    PlayFair pf(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^`abcdefghijklmnopqrstuvwxyz{|}~"); //! vocabulary _ is added as special character

    pf.print_vocabulary();

    string key;
    cout << endl<< "Enter Key" << endl;
    getline(cin, key);

    pf.BuildMatrixFromKey(key); // key
    pf.print_last_key();


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    string message;
    while (true) {

        cout << "Enter message" << endl;;
        getline(cin, message);

        pf.encrypt(message);
        string restored = pf.decrypt();
        pf.print_last_plaintext();
        pf.print_last_ciphertext();
        print_header("Restored");

        restored.erase(std::remove(restored.begin(), restored.end(), '_'), restored.end());
        cout << restored << endl;
    }
#pragma clang diagnostic pop

}

void run_rail_fence_demo() {
    print_header("Welcome To Rail Fence");

    int key;
    cout << endl << "Enter Key" << endl;
    cin >> key;
    railFenceTrans kt(key); //depth of fence
    scanf("%*c");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    string message;
    while (true) {
        cout << "Enter message" << endl;;
        getline(cin, message);
        kt.encrypt(message);

        kt.print_last_plaintext_line();
        kt.print_last_key();
        kt.print_last_ciphertext_line();
        print_header("Restored");
        cout << kt.decrypt() << endl;
    }
#pragma clang diagnostic pop

}


void run_RSA_demo() {
    print_header("Welcome To RSA");

    RSA rsa(100, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^`abcdefghijklmnopqrstuvwxyz{|}~");
    rsa.print_all_values();
    rsa.print_vocabulary();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    string message;

    while (true) {
        cout << endl<< "Enter message" << endl;;
        getline(cin, message);

        rsa.encrypt(message);

        rsa.print_last_plaintext_line();
        rsa.print_last_key();
        rsa.print_last_ciphertext_line();

        print_header("Restored");
        cout << rsa.decrypt() << endl;
    }
#pragma clang diagnostic pop

}

void run_keyless_demo(){
    print_header("Welcome To Keyless Transposition");
    Keyless keyless;
    string message;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        cout << endl<< "Enter message" << endl;;
        getline(cin, message);

        keyless.encrypt(message);
        keyless.print_last_plaintext();
        keyless.print_last_ciphertext();

        print_header("Restored");
        cout << keyless.decrypt() << endl;
    }
#pragma clang diagnostic pop

}

int main() {
    cout << "_ is special character in my algorithms don't use it :O"<<endl;
    //run_play_fair_demo();
    //run_rail_fence_demo();
    run_RSA_demo();
    //run_keyless_demo();
    return 0;
}