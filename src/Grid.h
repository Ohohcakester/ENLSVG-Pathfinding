#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <vector>

class Grid {
public:
    int sizeX;
    int sizeY;
    int totalSize;
    std::vector<std::vector<bool>> blocked;

    Grid(int sizeX, int sizeY);

    inline bool isBlocked(int x, int y) const {
        return x < 0 || y < 0 || x >= sizeX || y >= sizeY || blocked[y][x];
    }

    inline void setBlocked(int x, int y, bool value) {
        blocked[y][x] = value;
    }

    inline bool isOuterCorner(int x, int y) const {
        return (isBlocked(x,y) || isBlocked(x+1,y+1)) != (isBlocked(x+1,y) || isBlocked(x,y+1));
    }

    void printGrid() const;
};


#endif