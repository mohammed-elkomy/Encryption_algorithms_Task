//!
//! Created by Mohammed Alaa Elkomy on 22-Apr-18.
//! credits http://codeforces.com/blog/entry/22566
//! i got some snippets from the blog post
//! testing big mod exponentation http://comnuan.com/cmnn02/cmnn02008/cmnn02008.php

#ifndef ENCRYPTION_ALGORITHMS_BITINT_H
#define ENCRYPTION_ALGORITHMS_BITINT_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <deque>
#include <sstream>

using namespace std;

// base and base_digits must be consistent

int base_digits = 9;
long long base = static_cast<long long int>(1e9);//1000000000;

#define NEGATIVE (-1)
#define POSITIVE 1

#define __ONE__ BigInt(1)

class BigInt {
public:
    //deque<long long> vector_value;//! 0 is the least significant digit (to print you need to reverse)
    vector<long long> vector_value;//! 0 is the least significant digit (to print you need to reverse)
    int sign;

public:
    BigInt() :  //! default constructor
            sign(POSITIVE) {
    }

    explicit BigInt(long long v) {  //! constructor for long long
        *this = v;
    }

    explicit BigInt(const string &s) {  //! constructor for string
        to_bigNum(s);
    }

    BigInt &operator=(const BigInt &v) { //! copy the source bigint
        sign = v.sign;
        vector_value = v.vector_value;

        return *this;
    }

    BigInt &operator=(long long v) { //! convert to bigint base
        sign = POSITIVE;
        if (v < 0)
            sign = NEGATIVE, v = -v;
        vector_value.clear();
        for (; v > 0; v /= base)
            vector_value.push_back(static_cast<long long &&>(v % base)); //! pushing least significant first which is obtained by mod

        return *this;
    }

    BigInt operator+(const BigInt &that) const { //! addition operator overloading
        if (sign == that.sign) {
            BigInt res = that; //! initialize with THAT, even both are negatives this will work

            //! keep looping for all digits until exhausted considering the carry (go from the least significant digit)
            for (int i = 0, carry = 0; i < (int) max(vector_value.size(), that.vector_value.size()) || carry; ++i) {

                if (i == (int) res.vector_value.size())
                    res.vector_value.push_back(0); //! due to carry the result may be n+1

                //! accumulate to this digit
                res.vector_value[i] += carry + (i < (int) vector_value.size() ? vector_value[i] : 0);

                //! make a carry to the next digit if sum can't fit in this digit
                //! same as carry = sum / base
                carry = res.vector_value[i] >= base;

                if (carry)
                    res.vector_value[i] -= base; //!keep only the remainder = sum % base
            }

            return res;
        }

        //! A+B have different signs represent them as difference
        return *this - (-that);
    }

    BigInt operator-(const BigInt &that) const { //! subtraction operator overloading
        if (sign == that.sign) {
            //! assuming this >= that to implement borrow
            if (abs() >= that.abs()) {
                BigInt res = *this;

                //! keep looping for all digits until exhausted considering the carry
                for (int i = 0, borrow = 0; i < (int) that.vector_value.size() || borrow; ++i) {
                    res.vector_value[i] -= borrow + (i < (int) that.vector_value.size() ? that.vector_value[i] : 0);

                    borrow = res.vector_value[i] < 0; //! if we have borrow this will be negative, borrow it from the next digit

                    if (borrow)
                        res.vector_value[i] += base; //! take the borrow we are guaranteed to find borrow  since a>=b
                }

                return res.trim();
            }
            //!-ve as common factor
            return -(that - *this); //! reorder
        }

        //! A-B have different signs represent them as addition
        return *this + (-that);
    }


    BigInt operator*(long long val) const {  //! multiplication operator overloading for long long
        BigInt result((*this));
        long long int carry = 0;
        for (int i = 0; i < vector_value.size() || carry; i++) {
            if (i == (int) result.vector_value.size())
                result.vector_value.push_back(0); //! due to carry the result may be n+1

            result.vector_value[i] = (i < vector_value.size() ? (vector_value[i] * val) : 0) + carry;
            //! make a carry to the next digit if sum can't fit in this digit
            //! same as carry = sum / base
            carry = result.vector_value[i] / base;

            if (carry)
                result.vector_value[i] %= base; //!keep only the remainder = sum % base
        }
        return result.trim();
    }

    BigInt operator^(BigInt that) const {  //! power operator overloading (not bitwise xor)
        if (that.isZero()) //! power by that equal to zero
            return __ONE__;

        BigInt result;
        pair<BigInt, long long> halfDiv = that.Half();
        result = (*this) ^ halfDiv.first;
        result = result * result;
        if (halfDiv.second)
            result = result * (*this);
        return result;
    }

    BigInt powMod(BigInt that, const BigInt &Mod) const {  //! power mod
        if (that.isZero()) //! power by that equal to zero
            return __ONE__;

        BigInt result;
        pair<BigInt, long long> halfDiv = that.Half();
        result = (*this).powMod(halfDiv.first, Mod);
        result = result * result;
        if (halfDiv.second)
            result = result * (*this);
        return result % Mod;

        /* //! this is slower 1 sec to 15 sec
          BigInt result(__ONE__);
          BigInt THIS(*this);
          pair<BigInt, int> HALF;
          THIS = THIS % Mod;
          // equal to p
          while (that>__ZERO__)
          {
              HALF = that.Half();
              // If y is odd, multiply x with result
              if (HALF.second)
                  result = (result*THIS);

              // y must be even now
              that = HALF.first; // y = y/2
              THIS = (THIS*THIS) % Mod;
          }
          return result% Mod;*/
    }

    pair<BigInt, long long> Half() { //! divide by 2
        BigInt result((*this));
        long long remainder = 0;
        long long temp;
        for (long long i = result.vector_value.size() - 1; i >= 0; i--) {
            temp = (result.vector_value[i] + remainder * base) >> 1;
            //! IMPORTAAAAAAAAAAANT to be mathematically correct you need to add remainder * base but they aren't the least significant digit i need
            //! refer to / by long long
            remainder = result.vector_value[i] & 1;
            result.vector_value[i] = temp;
        }
        return make_pair(result.trim(), remainder);
    }

    BigInt Double() { //! mutiply by 2
        BigInt result((*this));
        int carry = 0;
        for (int i = 0; i < vector_value.size() || carry; i++) {
            if (i == (int) result.vector_value.size())
                result.vector_value.push_back(0); //! due to carry the result may be n+1

            result.vector_value[i] = (i < vector_value.size() ? (vector_value[i] << 1) : 0) + carry; //! loooooooooool i need parenthesis :D :D !!!!!! i always miss this operator precedence
            //! make a carry to the next digit if sum can't fit in this digit
            //! same as carry = sum / base
            carry = result.vector_value[i] >= base;

            if (carry)
                result.vector_value[i] -= base; //!keep only the remainder = sum % base
        }
        return result;
    }

    BigInt operator/(long long val) const { //! divide by val
        return divModValue(val).first;
    }

    long long operator%(long long val) const { //! divide by val
        return divModValue(val).second;
    }

    pair<BigInt, long long> divModValue(long long val) const { //! divide by val
        auto num = BigInt(*this).to_string();

        BigInt result((*this));
        long long remainder = 0;
        long long temp;
        for (auto i = static_cast<int>(result.vector_value.size() - 1); i >= 0; i--) {
            temp = (result.vector_value[i] + remainder * base);
            remainder = temp % val;
            result.vector_value[i] = temp / val;
        }
        pair<BigInt, long long> ret (result.trim(), remainder);
        return ret;
    }

    //! <quotient,remainder>
    friend pair<BigInt, BigInt> divModBig(BigInt numerator, BigInt denominator) {
        //! division via binary search .. just for fun .. not really efficient being in logarithmic space
        /*  int sign = numerator.sign * denominator.sign;
          numerator.sign = denominator.sign = 1;
          if (numerator < denominator) return make_pair(__ZERO__, numerator);
          else if (numerator == denominator) return make_pair(__ONE__, __ZERO__);
          else { //! binary search division
              //! numerator < q * denominator ..00000001111111.. i need the last zero
              BigInt mid, low = __ZERO__, high = numerator;
              while (low < high) {
                  mid = low + (high - low + __ONE__).Half().first;
                  BigInt temp = mid * denominator;
                  if (numerator < temp)
                      high = mid - __ONE__;
                  else if (numerator > temp)
                      low = mid;//+ __ONE__;
                  else
                      return make_pair(mid, numerator - mid * denominator); //! the condition may hold on the edge so i wont subtract 1
              }

              return make_pair(low, numerator - low * denominator);
          }
         */


        //! binary search on the value space is very slow :(
        //!!!!!!!!!! back is the most significant
        long long norm = base / (denominator.vector_value.back() + 1);  //! norm is just a scalar for faster computation ... plus one wont make it over flow if 10000
        //! norm = 1000; all values will work but just slower
        BigInt newNumerator = numerator.abs() * norm;
        BigInt newDenominator = denominator.abs() * norm;
        BigInt quotient, remainder_;
        quotient.vector_value.resize(newNumerator.vector_value.size());

        for (auto i = static_cast<int>(newNumerator.vector_value.size() - 1); i >= 0; i--) {
            remainder_ *= base;
            remainder_ += newNumerator.vector_value[i];
            long long high_block_rem = remainder_.vector_value.size() <= newDenominator.vector_value.size() ? 0 : remainder_.vector_value[newDenominator.vector_value.size()];
            long long low_block_rem = remainder_.vector_value.size() <= newDenominator.vector_value.size() - 1 ? 0 : remainder_.vector_value[newDenominator.vector_value.size() - 1];
            long long atLeastDiv = (base * high_block_rem + low_block_rem) / newDenominator.vector_value.back();

            //! first method using linear search
            remainder_ -= newDenominator * atLeastDiv;

            while (remainder_.sign == NEGATIVE)
                remainder_ += newDenominator, --atLeastDiv;


            //! second method using binary search
            /*long long mid, low = 0, high = atLeastDiv;
            while (low < high) {
                mid = low + (high - low + 1)/2; //! add 1 to make sure it wont remain mid value
                BigInt temp = newDenominator * mid;
                if (remainder_ < temp)
                    high = mid - 1;
                else if (remainder_ > temp)
                    low = mid;//! the condition may hold on the edge so i wont add 1
            }
            atLeastDiv = low;
            remainder_ -= newDenominator * atLeastDiv;*/

            quotient.vector_value[i] = atLeastDiv;
        }

        quotient.sign = numerator.sign * denominator.sign;
        remainder_.sign = numerator.sign;
        quotient.trim();
        remainder_.trim();

        return make_pair(quotient,/*numerator-quotient*denominator*/ (remainder_ / norm)); //! both will work
    }

    BigInt operator/(const BigInt &v) const {
        return divModBig(*this, v).first;
    }

    BigInt operator%(const BigInt &v) const {
        return divModBig(*this, v).second;
    }

    void operator+=(const BigInt &that) {
        *this = *this + that;
    }

    void operator+=(const long long &num) {
        *this = *this + BigInt(num);
    }

    void operator-=(const BigInt &that) {
        *this = *this - that;
    }

    void operator*=(const BigInt &that) {
        *this = *this * that;
    }

    void operator*=(const long long &num) {
        *this = *this * num;
    }

    void operator/=(BigInt &v) {
        *this = *this / v;
    }

    void operator/=(const long long &num) {
        *this = *this / num;
    }

    bool operator<(const BigInt &v) const {
        if (sign != v.sign)//! sign
            return sign < v.sign;
        if (vector_value.size() != v.vector_value.size()) //! size
            return vector_value.size() * sign < v.vector_value.size() * v.sign;
        for (auto i = static_cast<int>(vector_value.size() - 1); i >= 0; i--) //! value
            if (vector_value[i] != v.vector_value[i])
                return vector_value[i] * sign < v.vector_value[i] * sign;
        return false;
    }

    bool operator>(const BigInt &v) const {
        return v < *this;
    }

    bool operator<=(const BigInt &v) const {
        return !(v < *this);
    }

    bool operator>=(const BigInt &v) const {
        return !(*this < v);
    }

    bool operator==(const BigInt &v) const {
        return !(*this < v) && !(v < *this);
    }

    bool operator!=(const BigInt &v) const {
        return *this < v || v < *this;
    }

    BigInt trim() {
        while (!vector_value.empty() && vector_value.back() == 0) //! 001 will be 100 so we check from the back , it's reversed
            vector_value.pop_back();
        if (vector_value.empty())
            sign = POSITIVE;
        return *this;
    }

    bool isZero() const {
        return vector_value.empty() || (vector_value.size() == 1 && !vector_value[0]);
    }

    BigInt operator-() const {
        BigInt res = *this;
        res.sign = -sign;
        return res;
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sign *= res.sign;
        return res;
    }

    friend BigInt gcd(const BigInt &a, const BigInt &b) {
        return b.isZero() ? a : gcd(b, a % b);
    }

    void to_bigNum(const string &s) { //! assuming the number is syntactically right , i wont feed it into rgex
        //! init
        sign = POSITIVE;
        vector_value.clear();
        int first_digit_position = 0;

        //! get the sign
        while (first_digit_position < (int) s.size() && (s[first_digit_position] == '-' || s[first_digit_position] == '+')) {
            if (s[first_digit_position] == '-')
                sign = -sign;
            ++first_digit_position;
        }

        //!for base_digits chunk
        for (auto i = static_cast<int>(s.size() - 1); i >= first_digit_position; i -= base_digits) { //!guaranteed not to overflow

            long long block_value = 0;
            for (int j = max(first_digit_position, i - base_digits + 1); j <= i; j++)
                //!the least significant digit is on right but for the vector it's on the left
                block_value = block_value * 10 + s[j] - '0'; //!(56465)(13216)(13216) take the block in order >>

            vector_value.push_back(block_value); //!pushing to the left(of vector) the (least significant block on right (row string))
        }

        trim();
    }


    //!A non-member function can access the private and protected members of a class if it is declared a friend of that class.
    //! That is done by including a declaration of this external function within the class, and preceding it with the keyword friend:
    friend ostream &operator<<(ostream &stream, const BigInt &v) {
        //!send sign
        if (v.sign == NEGATIVE)
            stream << '-';

        //!get the last element
        stream << (v.vector_value.empty() ? 0 : v.vector_value.back());

        //!continue from the back
        for (int i = (int) v.vector_value.size() - 2; i >= 0; --i)
            stream << setw(base_digits)/*!width*/ << setfill('0')/*!empty placeholder*/ << v.vector_value[i]/*!number*/;
        return stream;
    }

    string to_string() {
        stringstream ss;
        //!send sign
        if (sign == NEGATIVE)
            ss << '-';

        //!get the last element
        ss << (vector_value.empty() ? 0 : vector_value.back());

        //!continue from the back
        for (int i = (int) vector_value.size() - 2; i >= 0; --i)
            ss << setw(base_digits)/*!width*/ << setfill('0')/*!empty placeholder*/ << vector_value[i]/*!number*/;

        return ss.str();
    }

    static vector<long long> convert_base(const vector<long long> &a, int old_digits, int new_digits) {
        vector<long long> p(static_cast<unsigned int>(max(old_digits, new_digits) + 1));
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); i++)
            p[i] = p[i - 1] * 10;
        vector<long long> res;
        long long cur = 0;
        int cur_digits = 0;
        for (long long i : a) {
            cur += i * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back(int(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back(cur);
        while (!res.empty() && !res.back())
            res.pop_back();
        return res;
    }

    typedef vector<long long> vll;

    static vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = a.size();
        vll res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += a[i] * b[j];
            return res;
        }

        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k);
        vll a2(a.begin() + k, a.end());
        vll b1(b.begin(), b.begin() + k);
        vll b2(b.begin() + k, b.end());

        vll a1b1 = karatsubaMultiply(a1, b1);
        vll a2b2 = karatsubaMultiply(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        vll r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); i++)
            r[i] -= a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            r[i] -= a2b2[i];

        for (int i = 0; i < (int) r.size(); i++)
            res[i + k] += r[i];
        for (int i = 0; i < (int) a1b1.size(); i++)
            res[i] += a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            res[i + n] += a2b2[i];
        return res;
    }

    BigInt operator*(const BigInt &v) const { //! multiplication operator overloading
        /* //! works with deque
         BigInt result;
         BigInt This = *this;
         for (long long &vectorValue : that.vector_value) {
             result += This * vectorValue;
             This.vector_value.push_front(0); //! shift the least significant
         }

         return result;*/
        //! log on the value space 24 ms :(
        /* //! log mul
          if (that == __ONE__)
              return (*this);

          BigInt result;
          pair<BigInt, int> halfDiv = that.Half();
          result = (*this) * halfDiv.first;
          result = result.Double();
          //result += result;
          if (halfDiv.second)
              result = result + (*this);
          return result;*/


        vector<long long> a6 = convert_base(this->vector_value, base_digits, 6);
        vector<long long> b6 = convert_base(v.vector_value, base_digits, 6);
        vll a(a6.begin(), a6.end());
        vll b(b6.begin(), b6.end());
        while (a.size() < b.size())
            a.push_back(0);
        while (b.size() < a.size())
            b.push_back(0);
        while (a.size() & (a.size() - 1))
            a.push_back(0), b.push_back(0);
        vll c = karatsubaMultiply(a, b);
        BigInt res;
        res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int) c.size(); i++) {
            long long cur = c[i] + carry;
            res.vector_value.push_back((int) (cur % 1000000));
            carry = (int) (cur / 1000000);
        }
        res.vector_value = convert_base(res.vector_value, 6, base_digits);
        res.trim();
        return res;
    }


};

void init();

bool miillerTest(BigInt d, BigInt n);

bool isPrime(const BigInt &n, int k);

BigInt random_bigint(int n);

BigInt random_bigPrime();

#include "BigInt.h"


const BigInt one(1);
const BigInt three(3);

vector<long long> seive;
vector<long long> primes;

uniform_int_distribution<> *dis = nullptr;
default_random_engine *gen = nullptr;


#define PRIMES 5000

void init() {
    seive.resize(PRIMES, 1); //! assume it's prime
    seive[0] = seive[1] = 0; //! not prime

    for (int i = 2; i < PRIMES; i++)
        if (seive[i]) //! if prime
            for (int j = i * 2; j < PRIMES; j += i)
                seive[j] = 0; //!multiples are not prime

    for (int i = 0; i < PRIMES; i++)
        if (seive[i]) //! if prime
            primes.push_back(i);

    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    gen = new default_random_engine(seed);
    dis = new uniform_int_distribution<>(0, 9);
}

//! https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/

//! This function is called for all k trials. It returns
//! false if n is composite and returns false if n is
//! probably prime.
//! d is an odd number such that  d*(2^r) = n-1
//! for some r >= 1
bool miillerTest(BigInt d, BigInt n) {
    //! Pick a random number in [2..n-2]
    //! Corner cases make sure that n > 4
    BigInt random = BigInt(2) + random_bigint(static_cast<int>(n.vector_value.size())) % (n - BigInt(4));

    BigInt N_minus_one = n - one;

    //! Compute a^d % n
    BigInt x = random.powMod(d, n);

    if (x == one || x == N_minus_one)
        return true;

    //! Keep squaring x while one of the following doesn't
    //! happen
    //! (i)   d does not reach n-1
    //! (ii)  (x^2) % n is not 1
    //! (iii) (x^2) % n is not n-1
    while (d != N_minus_one) {
        x = (x * x) % n;
        d *= 2;

        if (x == one) return false;
        if (x == N_minus_one) return true;
    }

    //! Return composite
    return false;
}

//! It returns false if n is composite and returns true if n
//! is probably prime.  k is an input parameter that determines
//! accuracy level. Higher value of k indicates more accuracy.
bool isPrime(const BigInt &n, int k = 64) //! false negative probability 2^-128
{
    //! Corner cases
    if (n <= one) return false; //! one or less
    else if (n <= three) return true; //! 2 or 3
    else if (!(n.vector_value[0] & 1)) return false;//! even and not 2

    for (long long prime : primes)
        if (n % prime == 0) //! is prime
            return false;

    //! Find the odd number d such that d*(2^r) + 1 = n  for some r >= 1
    BigInt d = n - one;
    //!Base cases make sure that n must be odd. Since n is odd, n-1 must be even. And an even number can be written as d * 2^s where d is an odd number and s > 0
    while (!(d.vector_value[0] & 1)) //! if we found any odd number it will be put as d
        d = d.Half().first;

    // !Iterate given n repeat of 'k' times
    for (int i = 0; i < k; i++)
        if (!miillerTest(d, n))
            return false;

    return true;
}


BigInt random_bigint(int n) {
    string s;
    for (int i = 0; i < n; i++)
        s += (*dis)(*gen) % 10 + '0';

    return BigInt(s);
}

BigInt random_bigPrime(int bit_size) {
    if (seive.empty())
        init();

    BigInt rand(random_bigint(bit_size / 3)); //! bit_size/3 decimal digits log2(10) ~ 3
    while (!isPrime(rand))
        rand = rand + one; //! bit_size/3 decimal digits
    return rand;
}

//! a*x + b*y =gcd
BigInt extended_gcd(const BigInt &a, const BigInt &b, BigInt &x, BigInt &y) {
    if (b == BigInt(0)) //!check on b for the zero division
    {
        x = 1, y = 0; //! a*1 + b *0 = gcd(a,0) =  a
        return a;
    }

    BigInt g = extended_gcd(b, a % b, y, x); //! b, a % b order for infinite loops
    y -= (a / b) * x;//!Ynew= Xold - (a/b) * Yold , Xnew=Yold
    return g;
}

//! since a*x + b*y =gcd
//! since e*x + phi*y =1 take mod phi
//! (e*x) mod phi = 1
BigInt multiplicative_inverse(const BigInt &e, const BigInt &phi) {
    BigInt x, y;
    BigInt g = extended_gcd(e, phi, x, y);
    if (g != BigInt(1))
        return BigInt(0);
    //throw invalid_argument("phi and public key pair have no multiplicative inverse");

    return (x + phi) % phi; //! for optimization and negatives
}

#endif //ENCRYPTION_ALGORITHMS_BITINT_H
