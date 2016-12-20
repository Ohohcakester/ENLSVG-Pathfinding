#ifndef _ENLSVG_EDGE_GRAPH_H_
#define _ENLSVG_EDGE_GRAPH_H_

#include "PathfindingDataTypes.h"
#include <iostream>
#include <vector>
#include <limits>
class Grid;
class LineOfSightScanner;

namespace ENLSVG {
    struct EdgeData {
        EdgeID oppositeEdge;
        const VertexID sourceVertex;
        const VertexID destVertex;
        const double weight;
        int level;
        std::vector<EdgeID> tautOutgoingEdges;

        inline bool isOriginal() const {return sourceVertex < destVertex;}

        EdgeData(VertexID sourceVertex, VertexID destVertex, double weight, int level)
        : sourceVertex(sourceVertex), destVertex(destVertex), weight(weight), level(level) {}
    };

    struct SkipEdge {
        const VertexID next;
        const double weight;
        const VertexID immediateNext; // immediate vertex just after current
        const VertexID immediateLast; // immediate vertex just before next.

        SkipEdge(VertexID next, double weight, VertexID imNext, VertexID imLast)
        : next(next), weight(weight), immediateNext(imNext), immediateLast(imLast) {}
    };

    struct MarkedEdges {
        std::vector<bool> isMarked;
        std::vector<EdgeID> markedIndexes;

        MarkedEdges(size_t nEdges) {
            isMarked.resize(nEdges, false);
        }

        inline void mark(EdgeID index) {
            isMarked[index] = true;
            markedIndexes.push_back(index);
        }

        inline void clear() {
            for (size_t i=0;i<markedIndexes.size();++i) {
                isMarked[markedIndexes[i]] = false;
            }
            markedIndexes.clear();
        }
    };

    class VisibilityGraph {

    public:
        VisibilityGraph(const Grid& grid, const LineOfSightScanner& scanner);
        void markEdgesFrom(MarkedEdges& markedEdges, const int sx, const int sy, const std::vector<GridVertex>& neighbours) const;
        void markBothWays(MarkedEdges& markedEdges) const;
        inline bool isSkipVertex(VertexID vertexID) const {return skipEdges[vertexID].size() > 0;}

        const int LEVEL_W = std::numeric_limits<VertexID>::max();
        const int sizeX;
        const int sizeY;

        // Indexed by VertexID
        std::vector<GridVertex> vertices;
        std::vector<std::vector<EdgeID>> edgeLists; // points to edge indexes.
        std::vector<std::vector<SkipEdge>> skipEdges;

        // Indexed by grid coordinates.
        std::vector<std::vector<VertexID>> nodeIndexes;

        // Indexed by EdgeID
        std::vector<EdgeData> edges;
        
    private:
        const Grid& grid;
        const LineOfSightScanner& scanner;

        inline EdgeID opposite(EdgeID edgeID) const {return edges[edgeID].oppositeEdge;}

        void connectEdge(int i, int j, int xi, int yi, int xj, int yj);
        void buildHierarchy();
        void computeAllEdgeLevels();
        void setupSkipEdges();
        void followLevelWPathToNextSkipVertex(EdgeID firstEdge,
            double& totalWeight, VertexID& nextVertex, VertexID& immediateNext,
            VertexID& immediateLast, const std::vector<bool>& isSkipVertex) const;
    };
}


#endif