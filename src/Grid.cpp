#include <iostream>
#include "Grid.h"
#include "RandomNumberGenerator.h"

Grid::Grid(int sizeX, int sizeY): sizeX(sizeX), sizeY(sizeY), totalSize(sizeX*sizeY) {
    blocked.resize(sizeY);
    for (size_t y=0;y<sizeY;++y) {
        blocked[y].resize(sizeX);
    }
}

void Grid::initialiseRandomGrid(float percentBlocked) {
    for (int y=0; y<sizeY; ++y) {
        for (int x=0; x<sizeX; ++x) {
            blocked[y][x] = global_rng.bernoulli(percentBlocked);
        }
    }
}

void Grid::printGrid() {
    for (int y=0;y<sizeY;++y) {
        for (int x=0;x<sizeX;++x) {
            std::cout << (blocked[y][x] ? "X" : " ");
        }
        std::cout << std::endl;
    }
}