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

void Drawer::drawLine(int x1 ,int y1, int x2, int y2, Colour& c) {
    TGAImage& img = *imgPtr;
    
    int dy = y2 - y1;
    int dx = x2 - x1;

    int f = 0;

    int signY = 1;
    int signX = 1;
    int offsetX = 0;
    int offsetY = 0;
    
    if (dy < 0) {
        dy *= -1;
        signY = -1;
        offsetY = -1;
    }
    if (dx < 0) {
        dx *= -1;
        signX = -1;
        offsetX = -1;
    }
    
    if (dx >= dy) {
        while (x1 != x2) {
            f += dy;
            if (f >= dx) {
                img.setPixel(c, x1 + offsetX, y1 + offsetY);
                y1 += signY;
                f -= dx;
            }
            if (f != 0) img.setPixel(c, x1 + offsetX, y1 + offsetY);
            if (dy == 0) {
                img.setPixel(c, x1 + offsetX, y1);
                img.setPixel(c, x1 + offsetX, y1 - 1);
            }

            x1 += signX;
        }
    }
    else {
        while (y1 != y2) {
            f += dx;
            if (f >= dy) {
                img.setPixel(c, x1 + offsetX, y1 + offsetY);
                x1 += signX;
                f -= dy;
            }
            if (f != 0) img.setPixel(c, x1 + offsetX, y1 + offsetY);
            if (dx == 0) {
                img.setPixel(c, x1, y1 + offsetY);
                img.setPixel(c, x1 - 1, y1 + offsetY);
            }

            y1 += signY;
        }
    }
}

void Drawer::drawCircle(int cx, int cy, int r, Colour& c) {
    TGAImage& img = *imgPtr;
    
    for (int y=0;y<=r;++y) {
        for (int x=0;x<=r;++x) {
            if (x*x+y*y <= r*r) {
                img.setPixel(c,cx+x,cy+y);
                img.setPixel(c,cx-x,cy+y);
                img.setPixel(c,cx+x,cy-y);
                img.setPixel(c,cx-x,cy-y);
            }
        }
    }
}

void Drawer::drawRect(int x1, int y1, int x2, int y2, Colour& c) {
    TGAImage& img = *imgPtr;

    for (int y=y1;y<y2;++y) {
        for (int x=x1;x<=x2;++x) {
            img.setPixel(c,x,y);
        }
    }
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
    TGAImage& img = *imgPtr;


}