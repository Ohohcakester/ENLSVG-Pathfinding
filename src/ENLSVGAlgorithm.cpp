#include "ENLSVGAlgorithm.h"
#include "ENLSVGGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"
#include "IndirectHeap.h"
#include <algorithm>
#include <limits>

//const double POS_INF = std::numeric_limits<double>::infinity();

struct AStarData {
    bool visited = false;
    double edgeWeightToGoal = -1.0;
    int parent = -1;
    double distance = POS_INF;
};

ENLSVGAlgorithm::ENLSVGAlgorithm(const Grid& grid):
    grid(grid), scanner(grid), graph(grid, scanner) {
}


Path ENLSVGAlgorithm::computeSVGPath(const int sx, const int sy, const int ex, const int ey) {
    // START: SPECIAL CASES - Handle special cases first.
    if (sx == ex && sy == ey) {
        Path path;
        path.push_back(GridVertex(sx, sy));
        return path;
    } else if (grid.lineOfSight(sx, sy, ex, ey)) {
        Path path;
        path.push_back(GridVertex(sx, sy));
        path.push_back(GridVertex(ex, ey));
        return path;
    }
    // END: SPECIAL CASES


    const size_t nNodes = graph.vertices.size();
    std::vector<AStarData> nodes;
    nodes.resize(nNodes);
    IndirectHeap pq(nNodes);

    double goalDistance = POS_INF;
    int goalParent = -1;
    { // START: INITIALISATION
        ScannerStacks data;
        const int startIndex = graph.nodeIndexes[sy][sx];
        if (startIndex == -1) {
            // Case 1: Start vertex is not a VG vertex.
            scanner.computeAllDirNeighbours(data, sx, sy);
            const std::vector<GridVertex>& neighbours = data.neighbours;
            for (size_t i=0; i<neighbours.size(); ++i) {
                const GridVertex& vn = neighbours[i];
                int neighbour = graph.nodeIndexes[vn.y][vn.x];

                double dist = grid.euclideanDistance(sx, sy, vn.x, vn.y);
                pq.decreaseKey(neighbour, 0);
                nodes[neighbour].distance = 0;
            }
        } else {
            // Case 2: Start vertex is a VG vertex.
            pq.decreaseKey(startIndex, 0);
            nodes[startIndex].distance = 0;    
        }

        const int goalIndex = graph.nodeIndexes[ey][ex];
        if (goalIndex == -1) {
            // Case 1: goal vertex is not a VG vertex.
            scanner.computeAllDirNeighbours(data, ex, ey);
            const std::vector<GridVertex>& neighbours = data.neighbours;
            for (size_t i=0; i<neighbours.size(); ++i) {
                const GridVertex& vn = neighbours[i];
                int neighbour = graph.nodeIndexes[vn.y][vn.x];
                nodes[neighbour].edgeWeightToGoal = grid.euclideanDistance(vn.x, vn.y, ex, ey);
            }
        } else {
            // Case 2: goal vertex is a VG vertex.
            nodes[goalIndex].edgeWeightToGoal = 0;
        }
    } // END: INITIALISATION

    while (pq.size() > 0) {
        if (goalDistance <= pq.getMinValue()) {
            break; // Reached Goal.
        }
        int curr = pq.popMinIndex();
        double currDistance = nodes[curr].distance;
        nodes[curr].visited = true;

        const std::vector<OutgoingEdge>& neighbours = graph.edgeLists[curr];
        for (size_t i=0;i<neighbours.size();++i) {
            int dest = neighbours[i].dest;
            double weight = neighbours[i].weight;

            double destDistance = currDistance + weight;
            if (destDistance < nodes[dest].distance) {
                nodes[dest].parent = curr;
                nodes[dest].distance = destDistance;
                pq.decreaseKey(dest, destDistance + heuristic(dest, ex, ey));
            }
        }

        if (nodes[curr].edgeWeightToGoal != -1.0) {
            double destDistance = currDistance + nodes[curr].edgeWeightToGoal;
            if (destDistance < goalDistance) {
                goalDistance = destDistance; // heuristic of goal should be 0
                goalParent = curr;
            }
        }
    }

    return getPath(nodes, goalParent, sx, sy, ex, ey);
}


Path ENLSVGAlgorithm::getPath(const std::vector<AStarData>& nodes, int goalParent,
const int sx, const int sy, const int ex, const int ey) const {
    Path path;

    // no path found.
    if (goalParent == -1) return path;

    // If Goal vertex is not a VG vertex:
    if (graph.nodeIndexes[ey][ex] == -1) {
        path.push_back(GridVertex(ex,ey));
    }

    int curr = goalParent;
    while (curr != -1) {
        path.push_back(graph.vertices[curr]);
        curr = nodes[curr].parent;
    }

    // If Start vertex is not a VG vertex:
    if (graph.nodeIndexes[sy][sx] == -1) {
        path.push_back(GridVertex(sx,sy));
    }

    std::reverse(path.begin(), path.end());
    return path;
}