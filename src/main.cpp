#include <iostream>
#include "Grid.h"
#include "Drawer.h"
#include "RandomGridGenerator.h"
#include "ENLSVGAlgorithm.h"
#include "PathfindingDataTypes.h"

int main() {
    int sizeX = 200;
    int sizeY = 200;

    Grid grid(sizeX, sizeY);
    //RandomGridGenerator::generateRandomGrid(grid, 0.5f);
    RandomGridGenerator::generateAutomataGrid(grid, 0.5f, 5, 1.f);

    ENLSVGAlgorithm enlsvg(grid);

    const int sx = 10;
    const int sy = 10;
    const int ex = 190;
    const int ey = 130;
    ParentPtrs parentPtrs;
    Path path = enlsvg.computeSVGPath(sx, sy, ex, ey, &parentPtrs);

    Drawer drawer(grid, 5);
    drawer.drawGrid(grid);
    drawer.drawVisibilityGraph(enlsvg.graph);
    drawer.drawParentPointers(parentPtrs);
    drawer.drawPath(path);
    drawer.drawPoint(sx, sy);
    drawer.drawPoint(ex, ey);

    drawer.save("outputImage.tga");
}