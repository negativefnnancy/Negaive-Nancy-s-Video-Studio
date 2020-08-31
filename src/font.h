#ifndef FONT_H
#define FONT_H

#include <cairo.h>

/* holds all the information to describe how text should be rendered */
typedef struct font_t {

    /* size in pixels of a character on the bitmap */
    int width, height;

    /* when rendering, how many pixels apart the left edge of each glyph should be */
    int spacing;

    /* how many frames in the font animation (rows in the bitmap) */
    int frames;

    /* how many frames per second to run the animation */
    int fps;

    /* the cairo surface holding the font bitmap data */
    cairo_surface_t *surface;

} font_t;

/* destroy an instantiated font */
void destroy_font (font_t *font);

/* load a font from a bitmap file given specifications */
font_t *create_font (char *path,
                     int width, int height, int spacing,
                     int frames, int fps);

#endif /* FONT_H */
