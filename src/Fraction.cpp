#include <iostream>
#include <string>
#include <sstream>
#include "fraction.h"
using namespace std;
    
Fraction operator*(const Fraction& o, const Fraction& o2) {
    return Fraction(o.n*o2.n, o.d*o2.d);
}

Fraction operator/(const Fraction& o, const Fraction& o2) {
    return Fraction(o.n*o2.d, o.d*o2.n);
}

Fraction operator+(const Fraction& o, const Fraction& o2) {
    return Fraction(o.n*o2.d + o2.n*o.d, o.d*o2.d);
}

Fraction operator-(const Fraction& o, const Fraction& o2) {
    return Fraction(o.n*o2.d - o2.n*o.d, o.d*o2.d);
}

ostream& operator<< (ostream& stream, Fraction& obj) {
    return stream << obj.toString();
}


Fraction parseFraction(string s) {
    int i=s.find_first_of('/');
    if (i == string::npos) {
        int n;
        stringstream(s) >> n;

        return Fraction(n);
    } else {
        int n, d;
        stringstream(s.substr(0,i)) >> n;
        stringstream(s.substr(i+1)) >> d;

        return Fraction(n,d);
    }
}