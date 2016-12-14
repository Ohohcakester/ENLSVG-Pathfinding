#include <iostream>
#include "Grid.h"
#include "Drawer.h"

int main() {
    int sizeX = 100;
    int sizeY = 100;

    Grid grid(sizeX, sizeY);
    grid.initialiseRandomGrid(0.5f);
    
    Drawer::drawGrid(grid, "outputGrid.tga");
}