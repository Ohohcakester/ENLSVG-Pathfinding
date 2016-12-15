#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <vector>
#include <cmath>

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

    inline bool topRightOfBlockedTile(int x, int y) const {
        return isBlocked(x-1, y-1);
    }

    inline bool topLeftOfBlockedTile(int x, int y) const {
        return isBlocked(x, y-1);
    }

    inline bool bottomRightOfBlockedTile(int x, int y) const {
        return isBlocked(x-1, y);
    }

    inline bool bottomLeftOfBlockedTile(int x, int y) const {
        return isBlocked(x, y);
    }

    static inline double euclideanDistance(int x1, int y1, int x2, int y2) {
        int dx = x2-x1;
        int dy = y2-y1;
        return sqrt(dx*dx+dy*dy);
    }

    void printGrid() const;
};


#endif