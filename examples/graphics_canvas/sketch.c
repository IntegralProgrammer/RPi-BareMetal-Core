#include <stddef.h>
#include "../../src/video.h"
#include "../../src/graphics_canvas.h"
#include "../../src/gpio.h"

#include "../../src/video_1024x768.h"
videoOutput display;
graphicsCanvas canvas;

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

}

void loop()
{
    while(1) {}
}
