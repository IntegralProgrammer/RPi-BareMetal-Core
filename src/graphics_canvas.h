#define GRAPHICS_CANVAS_CHAR_WIDTH 8
#define GRAPHICS_CANVAS_CHAR_HEIGHT 8

typedef struct {
    unsigned int width;
    unsigned int height;
    void* pixels;
} graphicsCanvas;

typedef struct {
    unsigned char fg_r;
    unsigned char fg_g;
    unsigned char fg_b;
    unsigned char bg_r;
    unsigned char bg_g;
    unsigned char bg_b;
    unsigned char bg_transparent;
    unsigned char size;
} fontConfig;

graphicsCanvas getGraphicsCanvas_generic(void*, unsigned int, unsigned int);
void setPixelGraphicsCanvas(graphicsCanvas, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
void graphicsCanvasFillRectangle(graphicsCanvas, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
fontConfig graphicsCanvasDefaultFontConfig();
void graphicsCanvasDrawChar(graphicsCanvas, unsigned int, unsigned int, fontConfig, char);
void graphicsCanvasDrawString(graphicsCanvas, unsigned int, unsigned int, fontConfig, char*);
