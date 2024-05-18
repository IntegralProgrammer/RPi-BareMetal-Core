#define VIDEO_OUTPUT_WIDTH 1024
#define VIDEO_OUTPUT_HEIGHT 768

#define videoOutputInit(fb) videoOutputInit_generic(fb, VIDEO_OUTPUT_WIDTH, VIDEO_OUTPUT_HEIGHT)
#define clearDisplay(fb) clearDisplay_generic(fb, VIDEO_OUTPUT_WIDTH * VIDEO_OUTPUT_HEIGHT * 4)
#define setPixel(fb, x, y, r, g, b) setPixel_generic(fb, VIDEO_OUTPUT_WIDTH, VIDEO_OUTPUT_HEIGHT, x, y, r, g, b)
#define getGraphicsCanvas(pixels) getGraphicsCanvas_generic(pixels, VIDEO_OUTPUT_WIDTH, VIDEO_OUTPUT_HEIGHT)

typedef struct {
    unsigned char pixels[VIDEO_OUTPUT_WIDTH * VIDEO_OUTPUT_HEIGHT * 4];
} videoOutput;
