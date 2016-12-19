#include "ENLSVGEdgeAlgorithm.h"
#include "ENLSVGEdgeGraph.h"
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

ENLSVGEdgeAlgorithm::ENLSVGEdgeAlgorithm(const Grid& grid):
    grid(grid), scanner(grid), graph(grid, scanner) {
}


Path ENLSVGEdgeAlgorithm::computeSVGPath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs) const {
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
        {
            const int startIndex = graph.nodeIndexes[sy][sx];
            // Add all visible neighbours of start vertex
            scanner.computeAllDirNeighbours(data, sx, sy);
            const std::vector<GridVertex>& neighbours = data.neighbours;
            for (size_t i=0; i<neighbours.size(); ++i) {
                const GridVertex& vn = neighbours[i];
                int neighbour = graph.nodeIndexes[vn.y][vn.x];

                double dist = grid.euclideanDistance(sx, sy, vn.x, vn.y);
                nodes[neighbour].distance = dist;
                pq.decreaseKey(neighbour, dist + heuristic(neighbour, ex, ey));
            }
            if (startIndex != -1) {
                // IF start vertex is a VG node, add it too.
                pq.decreaseKey(startIndex, 0);
                nodes[startIndex].distance = 0;    
            }
        }
        {
            const int goalIndex = graph.nodeIndexes[ey][ex];
            // Add all visible neighbours of goal vertex
            scanner.computeAllDirNeighbours(data, ex, ey);
            const std::vector<GridVertex>& neighbours = data.neighbours;
            for (size_t i=0; i<neighbours.size(); ++i) {
                const GridVertex& vn = neighbours[i];
                int neighbour = graph.nodeIndexes[vn.y][vn.x];
                nodes[neighbour].edgeWeightToGoal = grid.euclideanDistance(vn.x, vn.y, ex, ey);
            }
            if (goalIndex != -1) {
                // IF goal vertex is a VG node, add it too.
                nodes[goalIndex].edgeWeightToGoal = 0;
            }
        }
    } // END: INITIALISATION

    while (pq.size() > 0) {
        if (goalDistance <= pq.getMinValue()) {
            break; // Reached Goal.
        }
        int curr = pq.popMinIndex();
        if (nodes[curr].visited) continue;
        const double currDistance = nodes[curr].distance;
        const int currParent = nodes[curr].parent;
        nodes[curr].visited = true;

        const std::vector<EdgeID>& neighbours = graph.edgeLists[curr];
        for (size_t i=0;i<neighbours.size();++i) {
            const auto& edge = graph.edges[neighbours[i]];
            int dest = edge.destVertex;
            double weight = edge.weight;

            double destDistance = currDistance + weight;
            if (destDistance < nodes[dest].distance && isTaut(currParent, curr, dest)) {
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

    if (parentPtrs != nullptr) setParentPointers(nodes, goalParent, sx, sy, ex, ey, parentPtrs);
    return getPath(nodes, goalParent, sx, sy, ex, ey);
}


Path ENLSVGEdgeAlgorithm::getPath(const std::vector<AStarData>& nodes, int goalParent,
const int sx, const int sy, const int ex, const int ey) const {
    Path path;

    // no path found.
    if (goalParent == -1) return path;

    // If the last vertex is not equal to the goal vertex.
    if (graph.nodeIndexes[ey][ex] != goalParent) {
        path.push_back(GridVertex(ex,ey));
    }

    int curr = goalParent;
    while (curr != -1) {
        path.push_back(graph.vertices[curr]);
        curr = nodes[curr].parent;
    }

    // If Start vertex is not equal to the first vertex.
    if (sx != path.back().x || sy != path.back().y) {
        path.push_back(GridVertex(sx,sy));
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void ENLSVGEdgeAlgorithm::setParentPointers(const std::vector<AStarData>& nodes,
int goalParent, int sx, int sy, int ex, int ey, ParentPtrs* parentPtrs) const {
    
    parentPtrs->goal = GridVertex(ex, ey);
    parentPtrs->goalParent = graph.vertices[goalParent];

    std::vector<GridVertex>& current = parentPtrs->current;
    std::vector<GridVertex>& parent = parentPtrs->parent;

    current.clear();
    parent.clear();
    for (size_t i=0; i<nodes.size(); ++i) {
        if (nodes[i].distance == POS_INF) continue;

        current.push_back(graph.vertices[i]);
        if (nodes[i].parent != -1) {
            parent.push_back(graph.vertices[nodes[i].parent]);
        } else {
            parent.push_back(GridVertex(sx, sy));
        }
    }
}