#ifndef _ENLSVG_ALGORITHM_H_
#define _ENLSVG_ALGORITHM_H_

#include "PathfindingDataTypes.h"
class Grid;
class ENLSVGGraph;

class ENLSVGAlgorithm {
public:
    ENLSVGAlgorithm(const Grid& grid);

    Path computePath(const GridVertex& start, const GridVertex& end);

private:
    const ENLSVGGraph& graph;
    const Grid& grid;
};


#endif
