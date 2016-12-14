#ifndef _ENLSVG_ALGORITHM_H_
#define _ENLSVG_ALGORITHM_H_

#include "PathfindingDataTypes.h"
#include "ENLSVGGraph.h"
class Grid;
class LineOfSightScanner;

class ENLSVGAlgorithm {
public:
    ENLSVGAlgorithm(const Grid& grid);

    Path computePath(const GridVertex& start, const GridVertex& end);

private:
    const LineOfSightScanner& scanner;
    const Grid& grid;
    const ENLSVGGraph graph;
};


#endif
