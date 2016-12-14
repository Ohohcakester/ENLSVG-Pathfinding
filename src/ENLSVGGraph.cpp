#include "ENLSVGGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"


ENLSVGGraph::ENLSVGGraph(const Grid& grid, const LineOfSightScanner& scanner):
    sizeX(grid.sizeX), sizeY(grid.sizeY), scanner(scanner) {

    for (int y=0;y<=sizeY;++y) {
        for (int x=0;x<=sizeX;++x) {
            if (grid.isOuterCorner(x,y)) {
                vertices.push_back(GridVertex(x,y));
            }
        }
    }
}