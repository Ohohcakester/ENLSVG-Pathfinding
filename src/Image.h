#ifndef __IMAGE_SAVER__
#define __IMAGE_SAVER__

/**
 * Credit goes to Daniel Beard's Programming Blog for the base code
 * https://danielbeard.wordpress.com/2011/06/06/image-saving-code-c/
 */

//includes
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


//data structures
struct Colour {
	unsigned char r,g,b,a;

	Colour() {}
	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a): r(r), g(g), b(b), a(a) {}
	Colour(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b), a(255) {}
};

namespace Colours {
    extern const Colour RED;
    extern const Colour ORANGE;
    extern const Colour YELLOW;
    extern const Colour GREEN;
    extern const Colour BLUE;
    extern const Colour PURPLE;
    extern const Colour MAGENTA;
    extern const Colour BLACK;
    extern const Colour WHITE;
    extern const Colour BROWN;
    extern const Colour LIGHTGREY;
    extern const Colour GREY;
    extern const Colour DARKGREY;
    extern const Colour CYAN;
    extern const Colour LIME;
    extern const Colour AQUA;
    extern const Colour TEAL;
    extern const Colour PINK;
}


class TGAImage {

public:

	//Constructor
	TGAImage();

	//Overridden Constructor
	TGAImage(short width, short height);

	//Set all pixels at once
	void setAllPixels(Colour *pixels);

    //set individual pixels
    void setPixel(Colour inputcolor, int xposition, int yposition);

    //set individual pixels, includes boundary check.
    void setPixelSafe(Colour inputcolor, int xposition, int yposition);

	void WriteImage(string filename);

//General getters and setters

	void setWidth(short width);
	void setHeight(short height);

	short getWidth();
	short getHeight();

private:

	//store the pixels
	Colour *m_pixels;

	short m_height;
	short m_width;

	//convert 2D to 1D indexing
	int convert2dto1d(int x, int y); 

	

};


#endif
