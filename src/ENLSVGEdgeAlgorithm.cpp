#include "ENLSVGEdgeAlgorithm.h"
#include "ENLSVGEdgeGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"
#include "IndirectHeap.h"
#include <algorithm>

//const double POS_INF = std::numeric_limits<double>::infinity();

using namespace ENLSVG;

namespace ENLSVG {
    // negatePar makes parent pointers negative to indicate that the parent is a skip-edge.
    // note that negatePar(negatePar(k)) == k.
    inline VertexID negatePar(VertexID k) {return -k-1;}
    // restorePar returns the parent pointer to its original value.
    inline VertexID restorePar(VertexID k) {return k < 0 ? -k-1 : k;}

    struct AStarData {
        bool visited = false;
        double edgeWeightToGoal = -1.0;
        VertexID parent = NO_PARENT;
        double distance = POS_INF;

        AStarData(): visited(false), edgeWeightToGoal(-1.0), parent(NO_PARENT), distance(POS_INF) {}
    };


    Algorithm::Algorithm(const Grid& grid):
        grid(grid), scanner(grid), graph(grid, scanner) {
    }


    Path Algorithm::computeSVGPath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs) const {
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
        int goalParent = NO_PARENT;
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



    Path Algorithm::computePath(const int sx, const int sy, const int ex, const int ey, ParentPtrs* parentPtrs) const {
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
        nodes.resize(nNodes, AStarData());
        IndirectHeap pq(nNodes);

        MarkedEdges markedEdges(graph.edges.size());
        double goalDistance = POS_INF;
        int goalParent = NO_PARENT;
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
                    // IF start vertex is a VG node, mark it as visited so we don't waste time on it.
                    nodes[startIndex].visited = true;
                }
                graph.markEdgesFrom(markedEdges, sx, sy, neighbours);
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
                    // IF goal vertex is a VG node, mark it as visited so we don't waste time on it.
                    nodes[goalIndex].visited = true;
                }
                graph.markEdgesFrom(markedEdges, ex, ey, neighbours);
            }
        } // END: INITIALISATION
        graph.markBothWays(markedEdges);

        while (pq.size() > 0) {
            if (goalDistance <= pq.getMinValue()) {
                break; // Reached Goal, or min value = POS_INF (can't find goal)
            }
            int curr = pq.popMinIndex();
            const double currDistance = nodes[curr].distance;
            const int currParent = restorePar(nodes[curr].parent);
            nodes[curr].visited = true;

            // Traverse marked edges
            const std::vector<EdgeID>& neighbours = graph.edgeLists[curr];
            for (size_t i=0;i<neighbours.size();++i) {
                const EdgeID edgeId = neighbours[i];
                if (!markedEdges.isMarked[edgeId]) continue;
                const auto& edge = graph.edges[edgeId];
                int dest = edge.destVertex;
                if (nodes[dest].visited) continue;
                double weight = edge.weight;

                double destDistance = currDistance + weight;
                if (destDistance < nodes[dest].distance && isTaut(currParent, curr, dest)) {
                    nodes[dest].parent = curr;
                    nodes[dest].distance = destDistance;
                    pq.decreaseKey(dest, destDistance + heuristic(dest, ex, ey));
                }
            }

            // Traverse skip edges
            const std::vector<SkipEdge>& skipEdges = graph.skipEdges[curr];
            for (size_t i=0;i<skipEdges.size();++i) {
                const SkipEdge& edge = skipEdges[i];
                int dest = edge.next;
                if (nodes[dest].visited) continue;
                double weight = edge.weight;

                double destDistance = currDistance + weight;
                if (destDistance < nodes[dest].distance && isTaut(currParent, curr, edge.immediateNext)) {
                    nodes[dest].parent = negatePar(edge.immediateLast);
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

        markedEdges.clear();
        if (parentPtrs != nullptr) setParentPointers(nodes, goalParent, sx, sy, ex, ey, parentPtrs);
        return getPath(nodes, goalParent, sx, sy, ex, ey);
    }




    Path Algorithm::getPath(const std::vector<AStarData>& nodes, int goalParent,
    const int sx, const int sy, const int ex, const int ey) const {
        Path path;

        // no path found.
        if (goalParent == NO_PARENT) return path;

        // If the last vertex is not equal to the goal vertex.
        if (graph.nodeIndexes[ey][ex] != goalParent) {
            path.push_back(GridVertex(ex,ey));
        }

        const int LEVEL_W = graph.LEVEL_W;
        VertexID prev = NO_PARENT;
        VertexID curr = goalParent;
        // Assumption: first edge from goal is not a skip-edge.
        // Loop invariant: prev is nonnegative.
        while (curr != NO_PARENT) {
            if (curr > 0) {
                // Normal parent
                path.push_back(graph.vertices[curr]);
                prev = curr;
                curr = nodes[curr].parent;
            } else {
                // Skip-edge parent
                curr = negatePar(curr);
                const auto& edges = graph.edges;

                // Set first edge.
                EdgeID currEdge;
                {
                    const auto& edgeList = graph.edgeLists[prev];
                    for (size_t i=0; i<edgeList.size(); ++i) {
                        const EdgeID id = edgeList[i];
                        if (edges[id].destVertex != curr) continue;
                        currEdge = id;
                        break;
                    }
                }

                // Follow level-W edges until you reach then ext skip-vertex.
                path.push_back(graph.vertices[curr]);
                while (!graph.isSkipVertex(edges[currEdge].destVertex)) {
                    const auto& tautOutgoingEdges = edges[currEdge].tautOutgoingEdges;

                    // Find next outgoing level-W edge.
                    for (size_t i=0; i<tautOutgoingEdges.size(); ++i) {
                        EdgeID nextEdge = tautOutgoingEdges[i];
                        if (edges[nextEdge].level == LEVEL_W) {
                            currEdge = nextEdge;
                            break;
                        }
                    }
                    path.push_back(graph.vertices[edges[currEdge].destVertex]);
                    // This should not infinite loop done correctly.
                }

                prev = edges[currEdge].destVertex;
                curr = nodes[prev].parent;
            }
        }

        // If Start vertex is not equal to the first vertex.
        if (sx != path.back().x || sy != path.back().y) {
            path.push_back(GridVertex(sx,sy));
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    void Algorithm::setParentPointers(const std::vector<AStarData>& nodes,
    int goalParent, int sx, int sy, int ex, int ey, ParentPtrs* parentPtrs) const {

        parentPtrs->goal = GridVertex(ex, ey);
        parentPtrs->goalParent = (goalParent == NO_PARENT) ? parentPtrs->goal : graph.vertices[restorePar(goalParent)];

        std::vector<GridVertex>& current = parentPtrs->current;
        std::vector<GridVertex>& parent = parentPtrs->parent;

        current.clear();
        parent.clear();
        for (size_t i=0; i<nodes.size(); ++i) {
            if (nodes[i].distance == POS_INF) continue;

            current.push_back(graph.vertices[i]);
            if (nodes[i].parent != NO_PARENT) {
                parent.push_back(graph.vertices[restorePar(nodes[i].parent)]);
            } else {
                parent.push_back(GridVertex(sx, sy));
            }
        }
    }
}