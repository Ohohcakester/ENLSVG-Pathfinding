#include "Drawer.h"
#include "Grid.h"
#include "Image.h"
#include "ENLSVGGraph.h"

Drawer::Drawer(const Grid& grid): sizeX(grid.sizeX), sizeY(grid.sizeY) {
    imgPtr = new TGAImage(sizeX, sizeY);
}

Drawer::~Drawer() {
    delete imgPtr;
}

void Drawer::save(const std::string& filename) {
    imgPtr->WriteImage(filename);
}

void Drawer::drawGrid(const Grid& grid) {
    TGAImage& img = *imgPtr;

    Colour c;
    for (int y=0; y<sizeY; ++y) {
        for (int x=0; x<sizeX; ++x) {
            c.r = c.g = c.b = (grid.blocked[y][x] ? 64 : 255);
            img.setPixel(c,x,y);
        }
    }
}

void Drawer::drawVisibilityGraph(const ENLSVGGraph& graph) {

}