#include "Grid.h"


Grid::Grid(int sizeX, int sizeY): sizeX(sizeX), sizeY(sizeY), totalSize(sizeX*sizeY) {
    blocked.resize(sizeY);
    for (size_t y=0;y<sizeY;++y) {
        blocked[y].resize(sizeX);
    }
}