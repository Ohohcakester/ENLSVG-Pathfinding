#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <string>
class Grid;
class TGAImage;
class ENLSVGGraph;

class Drawer {
public:
    Drawer(const Grid& grid);
    ~Drawer();

    void save(const std::string& filename);
    void drawGrid(const Grid& grid);
    void drawVisibilityGraph(const ENLSVGGraph& graph);

    TGAImage* imgPtr;
    const int sizeX;
    const int sizeY;
};


#endif