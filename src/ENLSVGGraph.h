#ifndef _ENLSVG_GRAPH_H_
#define _ENLSVG_GRAPH_H_

#include "PathfindingDataTypes.h"
#include <vector>
class Grid;
class LineOfSightScanner;

class ENLSVGGraph {
public:
    struct OutgoingEdge {
        int dest;
        double weight;
    };

    ENLSVGGraph(const Grid& grid, const LineOfSightScanner& scanner);

    const int sizeX;
    const int sizeY;
    std::vector<GridVertex> vertices;
    std::vector<std::vector<OutgoingEdge>> edgeLists;

private:
    const LineOfSightScanner& scanner;
};

#endif