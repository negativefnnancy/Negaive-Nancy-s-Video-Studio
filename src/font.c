#include <stdlib.h>

#include "font.h"

void destroy_font (font_t *font) {

    cairo_surface_destroy (font->surface);
    free (font);
}

font_t *create_font (char *path,
                     int width, int height, int spacing,
                     int frames, int fps) {

    font_t *font  = (font_t *) calloc (1, sizeof (font_t));
    font->surface = cairo_image_surface_create_from_png (path);
    font->width   = width;
    font->height  = height;
    font->spacing = spacing;
    font->frames  = frames;
    font->fps     = fps;
    return font;
}
