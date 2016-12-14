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

    inline bool isBlocked(int x, int y) {
        return blocked[y][x];
    }

    inline void setBlocked(int x, int y, bool value) {
        blocked[y][x] = value;
    }


};


#endif