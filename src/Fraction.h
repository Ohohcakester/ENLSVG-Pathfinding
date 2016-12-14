#ifndef FRACTION_H
#define FRACTION_H

#include <string>
#include <sstream>
using namespace std;

struct Fraction {
    int n, d;

    inline Fraction() {
        // Invalid
        this->n = 0;
        this->d = 0;
    }
        
    inline Fraction(int n, int d) {
        init(n, d);
    }

    inline Fraction(int n) {
        init(n, 1);
    }

    // Copy constructor
    inline Fraction (const Fraction &o) {
        if (o.d == 0) {
            // Invalid fraction copy.
            this->n = 0;
            this->d = 0;
            return;
        }
        init(o.n, o.d);
    }

    inline void init(int n, int d) {
        /*if (d == 0) {
            cout << "\nERROR: Zero denominator: " << n << + "/" << d << "\n";
            return;
        }*/
        const int g = gcd(n,d);
        // Denominators are strictly positive.
        if (d > 0) {
            this->n = n/g;
            this->d = d/g;
        } else {
            this->n = -n/g;
            this->d = -d/g;
        }
    }

    inline float toFloat() const {
        return float(n)/d;
    }

    inline string toString() const {
        stringstream a;
        a << n;
        if (d != 1)
            a << "/" << d;
        return a.str();
    }

    inline static int gcd(int a, int b) {
        return a == 0 ? b : gcd(b%a, a);
    }


    inline Fraction& operator*=(const Fraction& o) {
        init(n*o.n, d*o.d);
        return *this;
    }

    inline Fraction& operator/=(const Fraction& o) {
        init(n*o.d, d*o.n);
        return *this;
    }

    inline Fraction& operator+=(const Fraction& o) {
        init(n*o.d + o.n*d, d*o.d);
        return *this;
    }

    inline Fraction& operator-=(const Fraction& o) {
        init(n*o.d - o.n*d, d*o.d);
        return *this;
    }

    inline bool operator==(const Fraction &o) const {
        // simplest form is guaranteed unique.
        return (n == o.n) && (d == o.d);
    }

    inline bool operator!=(const Fraction &o) const {
        return !(*this == o);
    }

    inline bool operator<(const Fraction &o) const {
        // Note: denominators are POSITIVE.
        return o.d*n < o.n*d;
    }

    inline bool operator>(const Fraction &o) const {
        return o.d*n > o.n*d;
    }

    inline bool operator<=(const Fraction &o) const {
        return !(*this > o);
    }

    inline bool operator>=(const Fraction &o) const {
        return !(*this < o);
    }

    inline bool isPositive() const {
        return n > 0;
    }

    inline bool isNegative() const {
        return n < 0;
    }

    inline bool isZero() const {
        return n == 0;
    }

    inline bool isInvalid() const {
        return d == 0;
    }

};

Fraction operator*(const Fraction& o, const Fraction& o2);
Fraction operator/(const Fraction& o, const Fraction& o2);
Fraction operator+(const Fraction& o, const Fraction& o2);
Fraction operator-(const Fraction& o, const Fraction& o2);
ostream& operator<< (ostream& stream, Fraction& obj);

Fraction parseFraction(string s);

#endif