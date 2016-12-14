#ifndef _LINE_OF_SIGHT_SCANNER_H_
#define _LINE_OF_SIGHT_SCANNER_H_

#include "PathfindingDataTypes.h"
class Grid;

class LineOfSightScanner {
public:
    LineOfSightScanner(const Grid& grid);

    std::vector<GridVertex> computeNeighbours(const GridVertex& gridVertex) const;

private:
    const Grid& grid;
};

#endif