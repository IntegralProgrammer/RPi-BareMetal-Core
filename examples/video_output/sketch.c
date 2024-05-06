#include <stddef.h>
#include "../../src/video.h"
#include "../../src/gpio.h"
#include "image.h"

#include "../../src/video_1024x768.h"
videoOutput display;

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

    // Display graphics on the video output
    for (unsigned int x = 0; x < 1024; x++) {
        for (unsigned int y = 0; y < 768; y++) {
            if (y < 192) {
                // Red bar
                setPixel(&display, x, y, 255, 0, 0);
            } else if (y < 384) {
                // Green bar
                setPixel(&display, x, y, 0, 255, 0);
            } else if (y < 576) {
                // Blue bar
                setPixel(&display, x, y, 0, 0, 255);
            } else {
                // Yellow bar
                setPixel(&display, x, y, 255, 255, 0);
            }
        }
    }

    // Display the Pi symbol on the screen at 100, 100
    unsigned int img_pixel_ptr = 0;
    for (unsigned int y = 0; y < 375; y++) {
        for (unsigned int x = 0; x < 385; x++) {
            unsigned char r = image_rgb[img_pixel_ptr++];
            unsigned char g = image_rgb[img_pixel_ptr++];
            unsigned char b = image_rgb[img_pixel_ptr++];
            setPixel(&display, x + 100, y + 100, r, g, b);
        }
    }

}

void loop()
{
    while(1) {}
}
