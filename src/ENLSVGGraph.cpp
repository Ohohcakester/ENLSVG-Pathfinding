#include "ENLSVGGraph.h"
#include "LineOfSightScanner.h"
#include "Grid.h"


ENLSVGGraph::ENLSVGGraph(const Grid& grid, const LineOfSightScanner& scanner):
    sizeX(grid.sizeX), sizeY(grid.sizeY), scanner(scanner) {

    // Initialise vertices (outer corners).
    nodeIndexes.resize(sizeY);
    for (int y=0;y<=sizeY;++y) nodeIndexes[y].resize(sizeX, -1);
    for (int y=0;y<=sizeY;++y) {
        for (int x=0;x<=sizeX;++x) {
            if (grid.isOuterCorner(x,y)) {
                nodeIndexes[y][x] = vertices.size();
                vertices.push_back(GridVertex(x,y));
            }
        }
    }

    // Initialise SVG edges
    edgeLists.resize(vertices.size());
    for (size_t i=0;i<vertices.size();++i) {
        int cx = vertices[i].x;
        int cy = vertices[i].y;

        std::vector<GridVertex> neighbours = scanner.computeTautNeighbours(cx, cy);

        std::vector<OutgoingEdge>& edgeList = edgeLists[i];
        edgeList.resize(neighbours.size());
        for (size_t j=0;j<neighbours.size();++j) {
            int nx = neighbours[j].x;
            int ny = neighbours[j].y;

            int dest = nodeIndexes[ny][nx];
            edgeList[j].dest = dest;
            edgeList[j].weight = grid.euclideanDistance(cx,cy,nx,ny);
        }
    }
}