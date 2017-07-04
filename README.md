# ENLSVG-Pathfinding
![Pathfinding-Demo](https://cloud.githubusercontent.com/assets/5209333/26408031/4dcd5b54-40cf-11e7-842e-e43318cebe8d.gif)

An implementation of the Edge N-Level Sparse Visibility Graph algorithm for extremely fast optimal Any-Angle Pathfinding on grid maps. (on the order of a millisecond per path computation on 6000x6000 grids)

The ENLSVG algorithm requires a pre-processing step, which may take some time, depending on the size of the grid map.

Note: gif taken from the [ENLSVG Pathfinding Demo](https://github.com/Ohohcakester/PathfindingDemo)

License: [Unlicense](http://unlicense.org/) (i.e. do whatever you want with it)

ENLSVG Algorithm: [Paper (SoCS-17)](https://aaai.org/ocs/index.php/SOCS/SOCS17/paper/view/15790) and [Slides](http://ohoh.byethost7.com/Contents/Projects/AnyAnglePathfinding/ENLSVG_SoCS17.pdf)

# Building

- Include the repository in your project, and build the library using CMake.
- Alternatively, as this library has no dependencies, the laziest option would be to add all the files (header files in Pathfinding/ and source files in cpp/) into your project. Note that C++ 11 is required.

# Usage

Include this for everything you need:
```cpp
#include <Pathfinding/ENLSVG.h>
```

## Quick Start

Sample Code:
```cpp
// 1200 x 1000 tiles
int sizeX = 1200;
int sizeY = 1000;

// Create grid
Pathfinding::Grid grid(sizeX, sizeY);

// Generate random automata grid
Pathfinding::RandomGridGenerator::generateAutomataGrid(grid, 0.37f, 5, .15f);

// Preprocess the grid to build a visibility graph.
Pathfinding::ENLSVG::Algorithm algo(grid);

// Initialise a memory object for the algorithm. The algorithm uses this memory object for its path computations.
Pathfinding::ENLSVG::Memory memory(algo);

// Compute 10000 paths
for (int i=0; i<10000; ++i) {
  int startX = 0;
  int startY = 0;
  int endX = 1200;
  int endY = 1000;
  
  Pathfinding::Path path = algo.computePath(memory, startX, startY, endX, endY);
  
  // do something with the path
}
```

More detailed explanations are below.

## Grid Object
A **Grid** is simply a 2D boolean array. true for a blocked tile, and false for an unblocked tile.

A grid has two sets of coordinates: Tile coordinates and vertex coordinates. The below diagram shows the difference between the two.
* In the diagram, tile coordinates are in square brackets e.g. `[0,1]`, and vertex coordinates are in round brackets e.g. `(0,1)`
* This is a 2x2 grid. As shown, tile coordinates go from `[0,0]` to `[1,1]`, while vertex coordinates go from `(0,0)` to `(2,2)`.
```
  _______________________
 |(0,2)      |(1,2)      |(2,2)
 |           |           |
 |   [0,1]   |   [1,1]   |
 |           |           |
 |___________|___________|
 |(0,1)      |(1,1)      |(2,1)
 |           |           |
 |   [0,0]   |   [1,0]   |
 |           |           |
 |___________|___________|
  (0,0)       (1,0)       (2,0)
```


Useful API:
1. `Pathfinding::Grid grid(1200, 1000)`
    * Constructor for grid object. Creates grid of size 1200x1000 tiles.
2. `grid.setBlocked(x, y, blocked)`
    * Sets the tile (x, y) to the boolean value "blocked"
3. `grid.isBlocked(x, y)`
    * Returns true iff tile (x, y) is blocked.
4. `grid.euclideanDistance(x1, y1, x2, y2)`
    * Convenience function. Returns the euclidean distance between vertices (x1, y1) and (x2, y2).
5. `grid.lineOfSight(x1, y1, x2, y2)`
    * returns true iff there is line of sight between vertices (x1, y1) and (x2, y2)
    * Note: (x1, y1) and (x2, y2) use vertex coordinates (Explained above)
  

Sample code for initialising a grid object:
```cpp
// 1200 x 1000 tiles
int sizeX = 1200;
int sizeY = 1000;

// Create grid
Pathfinding::Grid grid(sizeX, sizeY);

// Example: initialising the grid as a "checkerboard".
for (int y=0; y<sizeY; ++y) {
  for (int x=0; x<sizeX; ++x) {
    bool blocked = (x + y)%2 == 0;
    grid.setBlocked(x, y, blocked);
  }
}
```

## Automated Grid Generation
We provide two functions for generating random grids.

1. `Pathfinding::RandomGridGenerator::generateRandomGrid(Grid& grid, float percentBlocked)`
    * Generates a grid map where each tile is individually randomly chosen to be blocked or unblocked.
    * `Grid& grid`: The grid to generate the output onto
    * `float percentBlocked`: The probability of a tile being blocked

2. `Pathfinding::RandomGridGenerator::generateAutomataGrid(Grid& grid, float percentBlocked, int iterations, float resolutionMultiplier)`
    * Uses cellular automata to generate a cave-like map.
    * `Grid& grid`: The grid to generate the output onto
    * `float percentBlocked`: The percentage of blocked tiles
    * `int iterations`: The number of iterations of cellular automata to run. The more iterations, the smoother the map will be.
    * `float resolutionMultiplier`: The larger the resolutionMultiplier, the larger the "islands" will be. A smaller resolutionMultiplier will generate a map with many small details.

Sample code for initialising a random grid:
```cpp
// 1200 x 1000 tiles
int sizeX = 1200;
int sizeY = 1000;

// Create grid
Pathfinding::Grid grid(sizeX, sizeY);

// Generate random automata grid
Pathfinding::RandomGridGenerator::generateAutomataGrid(grid, 0.37f, 5, .15f);
```

## Data Types

**GridVertex**: a tuple of two ints that represents a grid vertex. This struct is self-explanatory.
```cpp
struct GridVertex {
    int x;
    int y;

    GridVertex() {}
    GridVertex(int x, int y): x(x), y(y) {}
};
```

**Path**: This typedef is self-explanatory.
```cpp
typedef std::vector<GridVertex> Path;
```

## ENLSVG Algorithm

The following function is used to compute a Path between two grid vertices:
```cpp
Pathfinding::Path Pathfinding::ENLSVG::Algorithm::computePath(Memory& memory, int sx, int sy, int ex, int ey) const;
```

The following example shows how it is used.

Sample usage:
```cpp
// We assume that we have a grid object "Pathfinding::Grid grid" like defined above.

// Preprocess the grid to build a visibility graph.
Pathfinding::ENLSVG::Algorithm algo(grid);

// Initialise a memory object for the algorithm. The algorithm uses this memory object for its path computations.
Pathfinding::ENLSVG::Memory memory(algo);

// Compute 10000 paths
for (int i=0; i<10000; ++i) {
  int startX = 0;
  int startY = 0;
  int endX = 1200;
  int endY = 1000;
  
  Pathfinding::Path path = algo.computePath(memory, startX, startY, endX, endY);
  
  // do something with the path
}
```

To make the best use out of the algorithm, it is recommended to store the `algo` object and the `memory` object to use for future computations. Initialising a Pathfinding::ENLSVG:Algorithm on a grid is a slow operation, and should not be repeated unless the grid has been modified.

The function `algo.computePath(...)` is thread-safe. However, a separate copy of the Pathfinding::ENLSVG::Memory object must be used for each thread, if you want to do path computations in parallel.

For example:
```cpp
// Stored Data
Pathfinding::ENLSVG::Algorithm algo(grid);
Pathfinding::ENLSVG::Memory memory1(algo);
Pathfinding::ENLSVG::Memory memory2(algo);
Pathfinding::ENLSVG::Memory memory3(algo);
```

```cpp
// Thread 1
Pathfinding::Path path = algo.computePath(memory1, startX, startY, endX, endY);
// do something with the path
```

```cpp
// Thread 2
Pathfinding::Path path = algo.computePath(memory2, startX, startY, endX, endY);
// do something with the path
```

```cpp
// Thread 3
Pathfinding::Path path = algo.computePath(memory3, startX, startY, endX, endY);
// do something with the path
```








