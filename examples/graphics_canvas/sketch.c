#include <stddef.h>
#include "../../src/video.h"
#include "../../src/graphics_canvas.h"
#include "../../src/gpio.h"

#include "../../src/video_1024x768.h"
videoOutput display;
graphicsCanvas canvas;

char* hello_world = "Hello, World!";
char* hello_rpi = "Hello, Raspberry Pi!";

void setup()
{
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);

    // Initialize the video output
    if (videoOutputInit(&display) != 0) {
        digitalWrite(7, LOW);
        return;
    }

    // Clear the display
    clearDisplay(&display);

    // Get a graphicsCanvas to draw on
    canvas = getGraphicsCanvas(&display);

    // Draw on the graphics canvas
    graphicsCanvasFillRectangle(canvas, 100, 200, 200, 80, 255, 0, 0); // Red rectangle
    graphicsCanvasFillRectangle(canvas, 300, 400, 150, 150, 0, 0, 255); // Blue rectangle
    graphicsCanvasFillRectangle(canvas, 400, 50, 150, 150, 0, 255, 0); // Green rectangle

    // Write some text on the graphics canvas
    fontConfig fc = graphicsCanvasDefaultFontConfig();
    graphicsCanvasDrawString(canvas, 180, 180, fc, hello_world);

    // Different size and color
    fc.size = 4;
    fc.fg_r = 255;
    fc.fg_g = 0;
    fc.fg_b = 0;
    fc.bg_r = 0;
    fc.bg_g = 0;
    fc.bg_b = 255;
    fc.bg_transparent = 0;
    graphicsCanvasDrawString(canvas, 300, 300, fc, hello_rpi);

    // Test printing all chars
    fc.size = 1;
    fc.fg_r = 255;
    fc.fg_g = 255;
    fc.fg_b = 255;
    fc.bg_transparent = 1;
    for (unsigned char c = 0; c < 256; c++) {
        unsigned int cx = GRAPHICS_CANVAS_CHAR_WIDTH * (c % 16);
        unsigned int cy = GRAPHICS_CANVAS_CHAR_HEIGHT * (c / 16);
        graphicsCanvasDrawChar(canvas, 350 + cx, 470 + cy, fc, c);
    }

}

void loop()
{
    while(1) {}
}
