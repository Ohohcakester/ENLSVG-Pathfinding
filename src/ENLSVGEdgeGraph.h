#ifndef _ENLSVG_EDGE_GRAPH_H_
#define _ENLSVG_EDGE_GRAPH_H_

#include "PathfindingDataTypes.h"
#include <iostream>
#include <vector>
class Grid;
class LineOfSightScanner;
typedef int VertexID;
typedef size_t EdgeID;

struct ENLSVGEdge {
    EdgeID oppositeEdge;
    VertexID sourceVertex;
    VertexID destVertex;
    double weight;
    int level;
    std::vector<EdgeID> tautOutgoingEdges;

    inline bool isOriginal() const {return sourceVertex < destVertex;}

    ENLSVGEdge(VertexID sourceVertex, VertexID destVertex, double weight, int level)
    : sourceVertex(sourceVertex), destVertex(destVertex), weight(weight), level(level) {}
};

struct SkipEdge {
    VertexID next;
    double weight;

    SkipEdge(VertexID next, double weight): next(next), weight(weight) {}
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
            isMarked[i] = false;
        }
        markedIndexes.clear();
    }
};

class ENLSVGEdgeGraph {

public:
    ENLSVGEdgeGraph(const Grid& grid, const LineOfSightScanner& scanner);
    void markEdgesFrom(MarkedEdges& markedEdges, const int sx, const int sy, const std::vector<GridVertex>& neighbours) const;
    void markBothWays(MarkedEdges& markedEdges) const;

    const int LEVEL_W = -1;
    const int sizeX;
    const int sizeY;

    // Indexed by VertexID
    std::vector<GridVertex> vertices;
    std::vector<std::vector<EdgeID>> edgeLists; // points to edge indexes.
    std::vector<std::vector<SkipEdge>> skipEdges;

    // Indexed by grid coordinates.
    std::vector<std::vector<VertexID>> nodeIndexes;

    // Indexed by EdgeID
    std::vector<ENLSVGEdge> edges;
    
private:
    const Grid& grid;
    const LineOfSightScanner& scanner;

    inline EdgeID opposite(EdgeID edgeID) const {return edges[edgeID].oppositeEdge;}
    inline EdgeID getOriginal(EdgeID edgeID) const {return edges[edgeID].isOriginal() ? edgeID : edges[edgeID].oppositeEdge;}
    inline bool isSkipVertex(VertexID vertexID) const {return skipEdges[vertexID].size() > 0;}

    void connectEdge(int i, int j, int xi, int yi, int xj, int yj);
    void buildHierarchy();
    void computeAllEdgeLevels();
    void setupSkipEdges();
    void followLevelWPathToNextSkipVertex(EdgeID firstEdge, double& totalWeight, VertexID& nextVertex, const std::vector<bool>& isSkipVertex) const;
};



#endif