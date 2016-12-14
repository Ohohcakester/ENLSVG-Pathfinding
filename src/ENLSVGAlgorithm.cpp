#include "ENLSVGAlgorithm.h"
#include "ENLSVGGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"

ENLSVGAlgorithm::ENLSVGAlgorithm(const Grid& grid):
    grid(grid), scanner(grid), graph(grid, scanner) {

}