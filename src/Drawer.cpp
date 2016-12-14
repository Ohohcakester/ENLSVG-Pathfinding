#include "Drawer.h"
#include "Grid.h"
#include "Image.h"

void Drawer::drawGrid(const Grid& grid, const std::string& filename) {
    int sizeX = grid.sizeX;
    int sizeY = grid.sizeY;

    TGAImage img(sizeX, sizeY);
    Colour c;

    for (int y=0; y<sizeY; ++y) {
        for (int x=0; x<sizeX; ++x) {
            c.r = c.g = c.b = (grid.blocked[y][x] ? 64 : 255);
            img.setPixel(c,x,y);
        }
    }

    img.WriteImage(filename);
}
