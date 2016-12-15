#include <iostream>
#include "Grid.h"
#include "Drawer.h"
#include "RandomGridGenerator.h"
#include "ENLSVGAlgorithm.h"

int main() {
    int sizeX = 200;
    int sizeY = 200;

    Grid grid(sizeX, sizeY);
    //RandomGridGenerator::generateRandomGrid(grid, 0.5f);
    RandomGridGenerator::generateAutomataGrid(grid, 0.5f, 5, 1.f);

    ENLSVGAlgorithm enlsvg(grid);
    //Drawer::drawGrid(grid, "outputGrid.tga");
}