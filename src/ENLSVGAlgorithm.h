#ifndef _ENLSVG_ALGORITHM_H_
#define _ENLSVG_ALGORITHM_H_

#include "PathfindingDataTypes.h"
#include "ENLSVGGraph.h"
#include <cmath>
class Grid;
class LineOfSightScanner;
struct AStarData;

class ENLSVGAlgorithm {

private:
    const LineOfSightScanner& scanner;
    const Grid& grid;
public:
    const ENLSVGGraph graph; //Note: This must be defined after scanner and grid.


    ENLSVGAlgorithm(const Grid& grid);

    Path computePath(const GridVertex& start, const GridVertex& end);
    Path computeSVGPath(const GridVertex& start, const GridVertex& end);

    inline double heuristic(int index, int ex, int ey) {
        int dx = graph.vertices[index].x - ex;
        int dy = graph.vertices[index].y - ey;
        return sqrt(dx*dx + dy*dy);
    }

private:
    Path getPath(const std::vector<AStarData>& nodes, int goalParent, const GridVertex& start, const GridVertex& goal) const;

};


#endif
