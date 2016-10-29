#include "canvas.h"
/*
#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

void Canvas::init(int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		exit(1);
	}
	screen = SDL_SetVideoMode(width , height, 32, SDL_DOUBLEBUF);
	if (screen == NULL) {
		exit(1);
	}
}

void Canvas::dispose() {
	SDL_Quit();
}

void Canvas::clear(color c) {
	SDL_FillRect(screen, NULL, c);
}

void Canvas::flip() {
	SDL_Flip(screen);
}

void Canvas::line(int x1, int y1, int x2, int y2, color c) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;
	int uy = ((dy > 0) << 1) - 1;
	int x = x1, y = y1, eps;

	eps = 0; dx = abs(dx); dy = abs(dy);
	if (dx > dy) {
		for (x = x1; x != x2; x += ux) {
			set_pixel(x, y, c);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy; eps -= dx;
			}
		}
	}
	else {
		for (y = y1; y != y2; y += uy) {
			set_pixel(x, y, c);
			eps += dx;
			if ((eps << 1) >= dy) {
				x += ux; eps -= dy;
			}
		}
	}
}

color Canvas::rgb(byte r, byte g, byte b) {
	return SDL_MapRGB(screen->format, r, g, b);
}

void Canvas::set_pixel(int x, int y, color c) {
	SDL_Surface *surface = screen;
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = c;
}
*/