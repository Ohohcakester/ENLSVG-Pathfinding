#ifndef _ENLSVG_EDGE_ALGORITHM_H_
#define _ENLSVG_EDGE_ALGORITHM_H_

#include "PathfindingDataTypes.h"
#include "ENLSVGEdgeGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"
#include <cmath>
#include <limits>
class Grid;

namespace ENLSVG {
    struct ENLSVGAStarData;

    // NO_PARENT should be positive to be immune to restorePar
    const VertexID NO_PARENT = std::numeric_limits<VertexID>::max();

    class ENLSVGEdgeAlgorithm {

    private:
        const Grid& grid;
        const LineOfSightScanner scanner;
    public:
        const ENLSVGEdgeGraph graph; //Note: This must be defined after scanner and grid.


        ENLSVGEdgeAlgorithm(const Grid& grid);

        Path computePath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs) const;
        Path computeSVGPath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs = nullptr) const;

        inline double heuristic(int index, int ex, int ey) const {
            int dx = graph.vertices[index].x - ex;
            int dy = graph.vertices[index].y - ey;
            return sqrt(dx*dx + dy*dy);
        }

        inline bool isTaut(int parent, int curr, int next) const {
            if (parent == NO_PARENT) return true;
            const std::vector<GridVertex>& vertices = graph.vertices;
            int x1 = vertices[parent].x;
            int y1 = vertices[parent].y;
            int x2 = vertices[curr].x;
            int y2 = vertices[curr].y;
            int x3 = vertices[next].x;
            int y3 = vertices[next].y;

            return grid.isTaut(x1, y1, x2, y2, x3, y3);
        }

    private:
        Path getPath(const std::vector<ENLSVGAStarData>& nodes, int goalParent, const int sx, const int sy, const int ex, const int ey) const;
        void setParentPointers(const std::vector<ENLSVGAStarData>& nodes, int goalParent, int sx, int sy, int ex, int ey, ParentPtrs* parentPtrs) const;
    };
}

#endif
