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

        OutgoingEdge() {}
        //OutgoingEdge(int dest, double weight): dest(dest), weight(weight) {}
    };

    ENLSVGGraph(const Grid& grid, const LineOfSightScanner& scanner);

    const int sizeX;
    const int sizeY;
    std::vector<GridVertex> vertices;
    std::vector<std::vector<OutgoingEdge>> edgeLists;
    std::vector<std::vector<int>> nodeIndexes;

private:
    const LineOfSightScanner& scanner;
};

#endif