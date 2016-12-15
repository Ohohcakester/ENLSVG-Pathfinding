#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <string>
class Grid;
class TGAImage;
class ENLSVGGraph;
class Colour;

class Drawer {
public:
    Drawer(const Grid& grid);
    ~Drawer();

    void save(const std::string& filename);
    void drawGrid(const Grid& grid);
    void drawVisibilityGraph(const ENLSVGGraph& graph);

    void drawLine(int x1 ,int y1, int x2, int y2, Colour& c);
    void drawCircle(int cx, int cy, int r, Colour& c);
    void drawRect(int x1, int y1, int x2, int y2, Colour& c);

    TGAImage* imgPtr;
    const int sizeX;
    const int sizeY;
};


#endif