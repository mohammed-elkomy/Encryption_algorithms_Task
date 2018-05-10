//
// Created by Mohammed Alaa Elkomy on 22-Apr-18.
//

#ifndef ENCRYPTION_ALGORITHMS_MYBADBIGINT_H
#define ENCRYPTION_ALGORITHMS_MYBADBIGINT_H


#include <algorithm>

#define NEGATIVE -1
#define POSITIVE 1

#define ZERO myBadBigInt("0")
#define ONE myBadBigInt("1")

using namespace std;

class myBadBigInt {

public:
    myBadBigInt() = default; //! default constructor
    myBadBigInt(string b) { (*this) = std::move(b); } //! constructor for string
    myBadBigInt(int size) { bitstring.resize(static_cast<unsigned int>(size)); } //! constructor for size
private:
    string bitstring; //! to store the digits
    int sign = POSITIVE; //! sign = NEGATIVE for negative numbers, sign = POSITIVE otherwise


public:
    int size() { //! returns number of digits
        return bitstring.size();
    }

    myBadBigInt inverseSign() { //! changes the sign
        sign *= -1;
        return (*this);
    }

    myBadBigInt normalize(int newSign) { //! removes leading 0, fixes sign
        for (int i = bitstring.size() - 1; i > 0 && (bitstring[i] == '0' || bitstring[i] == 0); i--)
            bitstring.erase(bitstring.begin() + i);
        sign = (bitstring.size() == 1 && bitstring[0] == '0') ? 1 : newSign;
        return (*this);
    }

    myBadBigInt &operator=(string b) { //! string assignment to myBadBigInt
        bitstring = b[0] == '-' ? b.substr(1) : b;//!take the magnitude part
        reverse(bitstring.begin(), bitstring.end());
        this->normalize(b[0] == '-' ? NEGATIVE : POSITIVE);//!feed the sign
        return *this;
    }

    bool operator<(const myBadBigInt &b) const { //! less than operator
        if (sign != b.sign) return sign < b.sign; //!different signs

        //! not equal size
        //! if signs are positive the larger in size is greater
        //! less for negative
        if (bitstring.size() != b.bitstring.size())
            return sign == 1 ? bitstring.size() < b.bitstring.size() : bitstring.size() > b.bitstring.size();

        //! equal size
        //! if signs are positive the larger is binary one digit
        //! binary zero digit for negative
        for (int i = bitstring.size() - 1; i >= 0; i--)
            if (bitstring[i] != b.bitstring[i])
                return sign == 1 ? bitstring[i] < b.bitstring[i] : bitstring[i] > b.bitstring[i];

        //! dummy default return
        return false;
    }

    bool operator==(const myBadBigInt &b) const { //! operator for equality
        //! both sign and bitstring value has to be  equal
        return bitstring == b.bitstring && sign == b.sign;
    }

    myBadBigInt operator+(myBadBigInt b) { //! addition operator overloading
        myBadBigInt maxi = ((*this).absol() < b.absol() ? b : *this);
        if (b.sign == NEGATIVE && sign == NEGATIVE) { //! if both negative
            auto result = (*this).absol() + b.absol();
            result.sign = NEGATIVE;
            return result;
        } else if (b.sign != sign && maxi.sign == NEGATIVE)  //! not equal sign and the largest magnitude is negative
            return ((*this).inverseSign() + b.inverseSign()).inverseSign();

        //! assumed here they both have the same sign or a >= b to let the borrow logic work
        myBadBigInt result(max(size(), b.size()) + 1);
        //! keep looping for all digits until exhausted considering the carry
        for (int i = 0, carry = 0; i < bitstring.size() || i < b.size() || carry; i++) {

            //! add to the carry .. the signed decimal digits expected 2 digit decimal value -9 to 18
            carry += sign * (i < bitstring.size() ? bitstring[i] - '0' : 0) + b.sign * (i < b.bitstring.size() ? b.bitstring[i] - '0' : 0);

            //! the sum digit
            result.bitstring[i] = static_cast<char>(((carry + 10) % 10) + '0');

            //! carry to the next step
            if (carry < 0)
                carry = -1;
            else
                carry /= 10;
        }

        //! normalize the result
        return result.normalize(maxi.sign);
    }

    myBadBigInt operator-(myBadBigInt b) { //! subtraction operator overloading
        return (*this) - std::move(b);
    }

    myBadBigInt operator*(myBadBigInt b) { //! multiplication operator overloading
/*
        myBadBigInt c("0");
        for (int i = 0, k = bitstring[i] - '0'; i < bitstring.size(); i++, k = bitstring[i] - '0') {
            while (k--)
                c = c + b; //! ith digit is k, so, we add k times
            b.bitstring.insert(b.bitstring.begin(), '0'); //! multiplied by 10
        }
        return c.normalize(sign * b.sign);*/

        //! repeat a addition for b times but with divide and conquer
        if (b == ONE)
            return (*this);

        myBadBigInt result;
        pair<myBadBigInt, int> halfDiv = b.half();
        result = (*this) * halfDiv.first;
        result = result + result;//+ (halfDiv.second ? (*this) : ZERO);
        if (halfDiv.second)
            result = result + (*this);
        return result.normalize(result.sign);
    }

    pair<myBadBigInt, int> half() { //! divide by 2
        myBadBigInt result((*this));
        int remainder = 0;
        char temp;
        for (int i = result.size() - 1; i >= 0; i--) {
            temp = static_cast<char>((result.bitstring[i] - '0' + remainder * 10) / 2 + '0');
            remainder = (result.bitstring[i] - '0') % 2;
            result.bitstring[i] = temp;
        }
        return make_pair(result.normalize(result.sign), remainder);
    }

    myBadBigInt Double() {
        int carry = 0, dbl;
        myBadBigInt result((*this));
        for (int i = 0; i < size() || carry > 0; i++) {
            dbl = (result.bitstring[i] - '0') * 2 + carry;
            result.bitstring[i] = static_cast<char>(dbl % 10 + '0');
            carry = dbl / 10;
        }
        return result;
    }

    myBadBigInt operator/(myBadBigInt b) { //! division operator overloading (binary search)


        //return d.normalize(dSign);
    }

    myBadBigInt operator%(myBadBigInt b) { //! modulo operator overloading
        if (b.size() == 1 && b.bitstring[0] == '0') b.bitstring[0] /= (b.bitstring[0] - '0');
        myBadBigInt c("0");
        b.sign = 1;
        for (int i = bitstring.size() - 1; i >= 0; i--) {
            c.bitstring.insert(c.bitstring.begin(), '0');
            c = c + bitstring.substr(i, 1);
            while (!(c < b)) c = c - b;
        }
        return c.normalize(sign);
    }

    myBadBigInt absol() {
        myBadBigInt result;
        result.bitstring = bitstring;
        return result;//! positive sign by default
    }

    void print() {
        if (sign == -1) putchar('-');
        for (int i = bitstring.size() - 1; i >= 0; i--) putchar(bitstring[i]);
    }
};


#endif //ENCRYPTION_ALGORITHMS_MYBADBIGINT_H
/*    string s1, s2;
    myBadBigInt a, b, c;
    *//*     while (cin >> s1 >> s2) {
             a = s1;
             b = s2;
             //c = a.half().first;
             //c.print();

             //return 0;
             c = a * b;
             c.print();
             cout << endl;
         }
  *//*

    for (int i = 0; i < 625; i++)
        s1 += "9";
    for (int i = 0; i < 625; i++)
        s2 += "9";
    a = s1;
    b = s2;
    int start_s = clock();
    c = a + b;
//freopen("output.txt", "w", stdout);
    a.print();
    cout << endl;
    b.print();
    cout << endl;
    c.print();
    cout << endl;
    int stop_s = clock();
    cout << "time in MS: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;

    a = "111";
    b = "999";
    (a + b).print();
    cout << endl;


    return 0;*/