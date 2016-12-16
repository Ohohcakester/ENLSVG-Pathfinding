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
        return (isBlocked(x-1,y-1) || isBlocked(x,y)) != (isBlocked(x,y-1) || isBlocked(x-1,y));
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

    inline bool lineOfSight(int x1, int y1, int x2, int y2) const {
        int dy = y2 - y1;
        int dx = x2 - x1;

        int f = 0;

        int signY = 1;
        int signX = 1;
        int offsetX = 0;
        int offsetY = 0;
        
        if (dy < 0) {
            dy *= -1;
            signY = -1;
            offsetY = -1;
        }
        if (dx < 0) {
            dx *= -1;
            signX = -1;
            offsetX = -1;
        }
        
        if (dx >= dy) {
            while (x1 != x2) {
                f += dy;
                if (f >= dx) {
                    if (isBlocked(x1 + offsetX, y1 + offsetY))
                        return false;
                    y1 += signY;
                    f -= dx;
                }
                if (f != 0 && isBlocked(x1 + offsetX, y1 + offsetY))
                    return false;
                if (dy == 0 && isBlocked(x1 + offsetX, y1) && isBlocked(x1 + offsetX, y1 - 1))
                    return false;
                
                x1 += signX;
            }
        }
        else {
            while (y1 != y2) {
                f += dx;
                if (f >= dy) {
                    if (isBlocked(x1 + offsetX, y1 + offsetY))
                        return false;
                    x1 += signX;
                    f -= dy;
                }
                if (f != 0 && isBlocked(x1 + offsetX, y1 + offsetY))
                    return false;
                if (dx == 0 && isBlocked(x1, y1 + offsetY) && isBlocked(x1 - 1, y1 + offsetY))
                    return false;
                
                y1 += signY;
            }
        }
        return true;
    }

    void printGrid() const;
};


#endif