typedef struct {
    unsigned int width;
    unsigned int height;
    void* pixels;
} graphicsCanvas;

graphicsCanvas getGraphicsCanvas_generic(void*, unsigned int, unsigned int);
void setPixelGraphicsCanvas(graphicsCanvas, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
void graphicsCanvasFillRectangle(graphicsCanvas, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
