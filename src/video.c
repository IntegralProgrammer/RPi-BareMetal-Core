#include <stddef.h>
#include "mailbox.h"
#include "video.h"
#include "graphics_canvas.h"

extern void PUT32(unsigned int, unsigned int);

unsigned char videoOutputInit_generic(void* pixels, unsigned int width, unsigned int height)
{
    // The framebufferInfo struct must be placed at an address where the 4 least significants bits are 0000
    struct framebufferInfo fb_info __attribute__((aligned(16)));

    fb_info.width = width;
    fb_info.height = height;
    fb_info.vwidth = width;
    fb_info.vheight = height;
    // Framebuffer pitch is the number of bytes that are in each row on screen (thank you https://jsandler18.github.io/extra/framebuffer.html)
    fb_info.pitch = 4 * width;
    fb_info.depth = 32;
    fb_info.pointer = pixels + 0x40000000;

    // Send a pointer to fb_info to the GPU adding 0x40000000 to translate CPU address space to GPU address space
    mailboxWrite((((unsigned int) &fb_info) + 0x40000000), FRAMEBUFFER_CHANNEL);

    // See if this call was sucessful or not
    unsigned int mailbox_data = mailboxRead(FRAMEBUFFER_CHANNEL);
    if (mailbox_data == 0) {
        // Failed to initialize framebuffer
        return 1;
    }
    return 0;
}

void clearDisplay_generic(void* pixels, unsigned int length)
{
    unsigned int pixel_base_ptr = (unsigned int) pixels;
    for (unsigned int i = 0; i < length; i++) {
        PUT32(pixel_base_ptr + i, 0);
    }
}

void setPixel_generic(void* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int pixel_ptr;
    unsigned int pixel;
    if (x >= width) {
        return;
    }
    if (y >= height) {
        return;
    }
    pixel = 0;
    pixel |= b;
    pixel |= g << 8;
    pixel |= r << 16;

    pixel_ptr = (unsigned int) pixels;
    pixel_ptr += (width * 4 * y);
    pixel_ptr += 4 * x;

    PUT32(pixel_ptr, pixel);
}

graphicsCanvas getGraphicsCanvas_generic(void* pixels, unsigned int width, unsigned int height)
{
    graphicsCanvas gc;
    gc.pixels = pixels;
    gc.width = width;
    gc.height = height;
    return gc;
}

void setPixelGraphicsCanvas(graphicsCanvas gc, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
    setPixel_generic(gc.pixels, gc.width, gc.height, x, y, r, g, b);
}
