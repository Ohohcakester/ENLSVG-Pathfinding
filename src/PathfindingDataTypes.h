#ifndef _PATHFINDING_DATA_TYPES_H_
#define _PATHFINDING_DATA_TYPES_H_

#include <vector>

struct GridVertex {
    int x;
    int y;

    GridVertex() {}
    GridVertex(int x, int y): x(x), y(y) {}
};

typedef std::vector<GridVertex> Path;

struct ParentPtrs {
    std::vector<GridVertex> current;
    std::vector<GridVertex> parent;
    GridVertex goal;
    GridVertex goalParent;
};

#endif