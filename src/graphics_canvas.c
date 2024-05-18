#include "graphics_canvas.h"

void graphicsCanvasFillRectangle(graphicsCanvas gc, unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b)
{
    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            setPixelGraphicsCanvas(gc, x0 + x, y0 + y, r, g, b);
        }
    }
}
