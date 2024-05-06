#define FRAMEBUFFER_CHANNEL 1

struct framebufferInfo {
    unsigned int width;
    unsigned int height;
    unsigned int vwidth;
    unsigned int vheight;
    unsigned int pitch;
    unsigned int depth;
    unsigned int ignorex;
    unsigned int ignorey;
    void * pointer;
    unsigned int size;
};

unsigned char videoOutputInit_generic(void*, unsigned int, unsigned int);
void clearDisplay_generic(void*, unsigned int);
void setPixel_generic(void*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
