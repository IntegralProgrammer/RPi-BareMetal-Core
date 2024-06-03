#include "graphics_canvas.h"
#include "graphics_canvas_font.h"

void graphicsCanvasFillRectangle(graphicsCanvas gc, unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b)
{
    for (unsigned int x = 0; x < w; x++) {
        for (unsigned int y = 0; y < h; y++) {
            setPixelGraphicsCanvas(gc, x0 + x, y0 + y, r, g, b);
        }
    }
}

fontConfig graphicsCanvasDefaultFontConfig()
{
    fontConfig fc;
    fc.fg_r = 255;
    fc.fg_g = 255;
    fc.fg_b = 255;
    fc.bg_r = 0;
    fc.bg_g = 0;
    fc.bg_b = 0;
    fc.bg_transparent = 1;
    fc.size = 1;
    return fc;
}

void graphicsCanvasDrawChar(graphicsCanvas gc, unsigned int x, unsigned int y, fontConfig fc, char c)
{
    // Calculate the byte offset into graphics_canvas_font_pixels for the given char
    unsigned char *char_pixels_ptr;
    if ((c < 32) || (c > 126)) {
        // Unprintable
        char_pixels_ptr = &graphics_canvas_font_pixels[0];
    } else {
        char_pixels_ptr = &graphics_canvas_font_pixels[0] + 8 + (8 * (c - 32));
    }

    // Set the pixels on the graphicsCanvas for this char
    for (unsigned int row = 0; row < 8; row++) {
        for (unsigned int col = 0; col < 8; col++) {
            unsigned char this_pixel = ((*char_pixels_ptr) >> col) & 1;
            if (this_pixel == 1) {
                graphicsCanvasFillRectangle(gc, x + (col * fc.size), y + (row * fc.size), fc.size, fc.size, fc.fg_r, fc.fg_g, fc.fg_b);
            } else {
                if (fc.bg_transparent == 0) {
                    graphicsCanvasFillRectangle(gc, x + (col * fc.size), y + (row * fc.size), fc.size, fc.size, fc.bg_r, fc.bg_g, fc.bg_b);
                }
            }
        }
        char_pixels_ptr++;
    }
}

void graphicsCanvasDrawString(graphicsCanvas gc, unsigned int x, unsigned int y, fontConfig fc, char *s)
{
    char c = *s;
    unsigned int xchar = x;
    while (c != '\0') {
        graphicsCanvasDrawChar(gc, xchar, y, fc, c);
        xchar += GRAPHICS_CANVAS_CHAR_WIDTH * fc.size;
        s++;
        c = *s;
    }
}
