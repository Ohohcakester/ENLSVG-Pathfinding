#ifndef _ENLSVG_ALGORITHM_H_
#define _ENLSVG_ALGORITHM_H_

#include "PathfindingDataTypes.h"
#include "ENLSVGGraph.h"
#include "LineOfSightScanner.h"
#include <cmath>
class Grid;
struct AStarData;

class ENLSVGAlgorithm {

private:
    const Grid& grid;
    const LineOfSightScanner scanner;
public:
    const ENLSVGGraph graph; //Note: This must be defined after scanner and grid.


    ENLSVGAlgorithm(const Grid& grid);

    Path computePath(const int sx, const int sy, const int ex, const int ey) const;
    Path computeSVGPath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs = nullptr) const;

    inline double heuristic(int index, int ex, int ey) const {
        int dx = graph.vertices[index].x - ex;
        int dy = graph.vertices[index].y - ey;
        return sqrt(dx*dx + dy*dy);
    }

private:
    Path getPath(const std::vector<AStarData>& nodes, int goalParent, const int sx, const int sy, const int ex, const int ey) const;
    void setParentPointers(const std::vector<AStarData>& nodes, int goalParent, int sx, int sy, int ex, int ey, ParentPtrs* parentPtrs) const;
};


#endif
