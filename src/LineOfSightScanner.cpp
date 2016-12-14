#include "LineOfSightScanner.h"
#include "Grid.h"
#include "Fraction.h"

//typedef double Rational;
typedef Fraction Rational;

struct ScanInterval {
    const Rational& xL;
    const Rational& xR;
    const bool leftInclusive;
    const bool rightInclusive;
    const int y;

    ScanInterval(const Rational& xL, const Rational& xR, bool leftInclusive, bool rightInclusive, int y):
        xL(xL), xR(xR), leftInclusive(leftInclusive), rightInclusive(rightInclusive), y(y) {}
};

LineOfSightScanner::LineOfSightScanner(const Grid& grid): grid(grid) {

}

std::vector<GridVertex> LineOfSightScanner::computeNeighbours(int px, int py) const {

}

std::vector<GridVertex> LineOfSightScanner::computeTautNeighbours(int px, int py) const {

}