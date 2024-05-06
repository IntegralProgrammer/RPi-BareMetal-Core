#define VIDEO_OUTPUT_WIDTH 640
#define VIDEO_OUTPUT_HEIGHT 480

#define videoOutputInit(fb) videoOutputInit_generic(fb, VIDEO_OUTPUT_WIDTH, VIDEO_OUTPUT_HEIGHT)
#define clearDisplay(fb) clearDisplay_generic(fb, VIDEO_OUTPUT_WIDTH * VIDEO_OUTPUT_HEIGHT * 4)
#define setPixel(fb, x, y, r, g, b) setPixel_generic(fb, VIDEO_OUTPUT_WIDTH, VIDEO_OUTPUT_HEIGHT, x, y, r, g, b)

typedef struct {
    unsigned char pixels[VIDEO_OUTPUT_WIDTH * VIDEO_OUTPUT_HEIGHT * 4];
} videoOutput;
