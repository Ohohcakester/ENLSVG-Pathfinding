#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <string>
class Grid;

class Drawer {
public:
    static void drawGrid(const Grid& grid, const std::string& filename);

};


#endif