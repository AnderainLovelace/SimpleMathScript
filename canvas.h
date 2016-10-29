#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "SDL/SDL.h"

typedef unsigned int color;
typedef unsigned char byte;

class Canvas {
public:
	void init(int scale_x, int scale_y);
	void dispose();
	void clear(color c = 0xffffff);
	void flip();
	void line(int x1, int y1, int x2, int y2, color c);
	color rgb(byte r, byte g, byte b);
private:
	SDL_Surface *screen;
	void set_pixel(int x, int y, color c);
};

#endif