#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <svgtiny.h>

#include "drawable.h"

void draw_drawable (drawable_t *drawable, cairo_t *cairo, double time) {

    /* transform by the transformation of each drawables */
    cairo_save (cairo);
    cairo_transform (cairo, &(drawable->transformation));

    /* polymorphic dispatch */
    drawable->draw (drawable, cairo, time);

    /* restore the transformation matrix */
    cairo_restore (cairo);
}

bool inside_drawable (drawable_t *drawable, vec2_t point) {

    vec2_t transformed;

    /* got to invert the trasnformation */
    cairo_matrix_t inverted = drawable->transformation;
    cairo_matrix_invert (&inverted);

    /* transform the point to the local coordinate system of the drawable */
    transformed = multiply_vec2_matrix (point, inverted);

    return drawable->inside (drawable, transformed);
}

void destroy_drawable (drawable_t *drawable) {

    /* polymorphic dispatch */
    drawable->destroy (drawable);

    free (drawable);
}

drawable_t *create_drawable (void *data,
                             drawable_draw_method_t *draw,
                             drawable_inside_method_t *inside,
                             drawable_destructor_t *destroy) {

    drawable_t *drawable = (drawable_t *) calloc (1, sizeof (drawable_t));
    drawable->data       = data;
    drawable->draw       = draw;
    drawable->inside     = inside;
    drawable->destroy    = destroy;
    cairo_matrix_init_identity (&(drawable->transformation));
    return drawable;
}

void draw_drawable_shape (drawable_t *drawable, cairo_t *cairo, double time) {

    drawable_shape_t *drawable_shape = (drawable_shape_t *) drawable->data;
    color_t color = drawable_shape->color;

    /* trace the shape */
    trace_shape (drawable_shape->shape, cairo);

    /* choose the desired color */
	cairo_set_source_rgba (cairo, color.r, color.g, color.b, color.a);

    /* paint the traced shape with the chosen color */
    /* TODO: stroke too? */
	cairo_fill (cairo);
}

bool inside_drawable_shape (drawable_t *drawable, vec2_t point) {

    drawable_shape_t *drawable_shape = (drawable_shape_t *) drawable->data;
    return inside_shape (drawable_shape->shape, point);
}

void destroy_drawable_shape (drawable_t *drawable) {

    free (drawable->data);
}

drawable_t *create_drawable_shape (shape_t *shape, color_t color) {

    drawable_shape_t *drawable_shape =
        (drawable_shape_t *) calloc (1, sizeof (drawable_shape_t));
    drawable_shape->shape = shape;
    drawable_shape->color = color;

    return create_drawable (drawable_shape,
                            draw_drawable_shape,
                            inside_drawable_shape,
                            destroy_drawable_shape);
}

void group_add_drawable (drawable_t *group, drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;
    drawable_group->drawables[drawable_group->n_drawables++] = drawable;
}

void group_remove_drawable (drawable_t *group, drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;
    remove_array_item ((void **) drawable_group->drawables,
                       &(drawable_group->n_drawables),
                       drawable);
}

void group_remove_all_drawables (drawable_t *group) {

    drawable_group_t *drawable_group = (drawable_group_t *) group->data;

    /* simply set the size of the array to 0 */
    drawable_group->n_drawables = 0;
}

void draw_drawable_group (drawable_t *drawable, cairo_t *cairo, double time) {

    int i;
    drawable_group_t *drawable_group = (drawable_group_t *) drawable->data;

    /* draw all the drawables */
    for (i = 0; i < drawable_group->n_drawables; i++)
        draw_drawable (drawable_group->drawables[i], cairo, time);
}

bool inside_drawable_group (drawable_t *drawable, vec2_t point) {

    int i;
    drawable_group_t *drawable_group = (drawable_group_t *) drawable->data;

    /* a point is inside a drawable group if it is in any
     * of the children drawables */

    for (i = 0; i < drawable_group->n_drawables; i++)
        if (inside_drawable (drawable_group->drawables[i], point))
            return true;

    return false;
}

void destroy_drawable_group (drawable_t *drawable) {

    drawable_group_t *drawable_group = (drawable_group_t *) drawable->data;

    free (drawable_group->drawables);
    free (drawable_group);
}

drawable_t *create_drawable_group () {

    drawable_group_t *drawable_group =
        (drawable_group_t *) calloc (1, sizeof (drawable_group_t));
    drawable_group->n_drawables = 0;
    drawable_group->drawables =
        (drawable_t **) calloc (DRAWABLE_GROUP_CAPACITY, sizeof (drawable_t *));

    return create_drawable (drawable_group,
                            draw_drawable_group,
                            inside_drawable_group,
                            destroy_drawable_group);
}

void draw_drawable_svg (drawable_t *drawable, cairo_t *cairo, double time) {

    drawable_svg_t *drawable_svg = (drawable_svg_t *) drawable->data;

    /* draw the root drawable */
    draw_drawable (drawable_svg->root, cairo, time);
}

bool inside_drawable_svg (drawable_t *drawable, vec2_t point) {

    drawable_svg_t *drawable_svg = (drawable_svg_t *) drawable->data;

    /* just check with the root, which contains all the components of the svg */
    return inside_drawable (drawable_svg->root, point);
}

void destroy_drawable_svg (drawable_t *drawable) {

    int i;
    drawable_svg_t *drawable_svg = (drawable_svg_t *) drawable->data;

    /* get the drawable group */
    drawable_group_t *drawable_group = (drawable_group_t *) drawable_svg->root->data;

    /* destroy all the shapes */
    for (i = 0; i < drawable_svg->n_shapes; i++)
        destroy_shape (drawable_svg->shapes[i]);

    /* destroy all the corresponding drawables */
    for (i = 0; i < drawable_group->n_drawables; i++)
        destroy_drawable (drawable_group->drawables[i]);

    destroy_drawable (drawable_svg->root);
    free (drawable_svg->shapes);
    free (drawable_svg);
}

drawable_t *create_drawable_svg (char *svg_path) {

    int i, j;
    struct svgtiny_diagram *diagram;
    svgtiny_code code;
    FILE *fp;      /* file pointer for the svg file */
    char *buffer;  /* loaded svg bytes from file */
    long int size; /* size of svg buffer */
    double half_width, half_height; /* half the size of the svg file canvas */

    drawable_svg_t *drawable_svg =
        (drawable_svg_t *) calloc (1, sizeof (drawable_svg_t));
    drawable_svg->shapes =
        (shape_t **) calloc (DRAWABLE_SVG_CAPACITY, sizeof (shape_t *));
    drawable_svg->root = create_drawable_group ();

    /* read the svg bytes from the file */
    if (!(fp = fopen (svg_path, "rb"))) {

        /* skip trying to parse anything if opening the file was a failure */
        fprintf (stderr, "Failed to open SVG file \"%s\"!\n", svg_path);
        goto done;
    }

    /* get the length of the file */
    fseek (fp, 0, SEEK_END);
    size = ftell (fp);
    rewind (fp);

    /* read in the buffer */
    buffer = (char *) malloc (size + 1);
    fread (buffer, 1, size, fp);
    fclose (fp);
    buffer[size] = 0; /* null terminator */

    /* parse the svg data */
    diagram = svgtiny_create ();

    /* diagram, svg data buffer, buffer size, source url?, viewport width, height? */
    debug_printf ("loaded %d bytes\n", size);
    code = svgtiny_parse (diagram, buffer, size, svg_path, 1, 1);
    half_width  = diagram->width  / 2.0;
    half_height = diagram->height / 2.0;

    /* don't need the buffer anymore */
    free (buffer);

    /* the svg file was parsed okay */
    if (code == svgtiny_OK) {

        /* iterate and create each shape in the svg */
        for (i = 0; i < diagram->shape_count; i++) {

            /* this means this shape is a path (not text or anything else)
             * rn we only recognize paths :o */
            if (diagram->shape[i].path) {

                drawable_t *drawable;
                color_t color;
                double x, y;

                /* add to the count of shapes in our representation */
                shape_t *shape = create_polygon ();
                drawable_svg->shapes[drawable_svg->n_shapes++] = shape;

                /* figure out the fill color
                 * (TODO: not currently supporting strokes) */
                if (diagram->shape[i].fill == svgtiny_TRANSPARENT)
                    color = make_color (0, 0, 0, 0);

                else
                    color = make_color (svgtiny_RED   (diagram->shape[i].fill) / 255.0,
                                        svgtiny_GREEN (diagram->shape[i].fill) / 255.0,
                                        svgtiny_BLUE  (diagram->shape[i].fill) / 255.0,
                                        1);

                /* create the corresponding shape and add it to the group */
                drawable = create_drawable_shape (shape, color);
                group_add_drawable (drawable_svg->root, drawable);

                /* iterate the path data */
                for (j = 0;
                     j < diagram->shape[i].path_length;
                     /* manual incrementing */) {

                    float *path = diagram->shape[i].path;

                    /* the first value indicates the meaning */
                    switch ((int) path[j]) {

                        /* currently not distuinguishing between these ... */
                        case svgtiny_PATH_MOVE:
                        case svgtiny_PATH_LINE:
                            /* normalize the size to unit half height
                             * and center it */
                            x = (path[j + 1] - half_width)  / half_height;
                            y = (path[j + 2] - half_height) / half_height;
                            add_vertex (shape, make_vec2 (x, y));
                            j += 3;
                            break;

                        /* TODO: we dont support this yet.... */
                        case svgtiny_PATH_CLOSE:
                            j += 1;
                            break;

                        /* TODO: we dont support these yet.... */
                        case svgtiny_PATH_BEZIER:
                            j += 7;
                            break;
                    }
                }
            }
        }
        
    } else
        fprintf (stderr, "Failed to parse SVG file \"%s\"!\n", svg_path);

    svgtiny_free (diagram);

done:
    return create_drawable (drawable_svg,
                            draw_drawable_svg,
                            inside_drawable_svg,
                            destroy_drawable_svg);
}

void draw_drawable_text (drawable_t *drawable, cairo_t *cairo, double time) {

    drawable_text_t *drawable_text = (drawable_text_t *) drawable->data;
    color_t color = drawable_text->color;
    char *character = drawable_text->string;
    font_t *font = drawable_text->font;
    cairo_surface_t *target_surface;
    cairo_format_t format;
    cairo_t *target_context;
    double scale;
    int frame;

    /* figure out what frame of animation to draw */
    frame = (int) (time * font->fps) % font->frames;

    /* scale down to one character per unit half height */
    cairo_save (cairo);
    scale = 1.0 / font->width;
    cairo_scale (cairo, scale, scale);

    /* create a surface to render the text to that is big enough for the whole string*/
    format = cairo_image_surface_get_format (font->surface);
    target_surface = cairo_image_surface_create (format,
                                                 font->width * strlen (character),
                                                 font->width);
    target_context = cairo_create (target_surface);

    /* iterate the characters to be drawn until the null terminator is reached */
    while (*character) {

        /* calculate the x offset in the font of the character
         * the font is 128 x 128 characters,
         * and the first character is ascii character #32 */
        int x = (*character - 32) * font->width;

        /* set the source pattern to the given font at the x offset
         * the y offset depends on the current frame of animation
         * because each row in the image is another frame */
        cairo_set_source_surface (target_context, font->surface,
                                  -x, -frame * font->width);

        /* draw the character glyph */
        /* TODO: text position anchors; rn this is top left only!! */
        cairo_rectangle (target_context, 0, 0, font->width, font->width);

        /* paint the character */
	    cairo_fill (target_context);

        /* translate to the position of the next character */
        cairo_translate (target_context, font->spacing, 0);

        /* point to the next character */
        character++;

    }

    /* color the letters! */
    cairo_set_operator (target_context, CAIRO_OPERATOR_IN);
    cairo_set_source_rgba (target_context, color.r, color.g, color.b, color.a);
    cairo_paint (target_context);

    /* now copy the rendered-to surface onto the real target ! */
    cairo_set_source_surface (cairo, target_surface, 0, 0);
    cairo_paint (cairo);

    /* clean up */
    cairo_destroy (target_context);
    cairo_surface_destroy (target_surface);

    /* restore the transformations */
    cairo_restore (cairo);
}

void destroy_drawable_text (drawable_t *drawable) {

    free (drawable->data);
}

bool inside_drawable_text (drawable_t *drawable, vec2_t point) {

    /* drawable_text_t *drawable_text = (drawable_text_t *) drawable->data; */

    /* TODO */
    return false;
}

drawable_t *create_drawable_text (char *string, color_t color, font_t *font) {

    drawable_text_t *drawable_text =
        (drawable_text_t *) calloc (1, sizeof (drawable_text_t));
    drawable_text->string = string;
    drawable_text->color = color;
    drawable_text->font = font;

    return create_drawable (drawable_text,
                            draw_drawable_text,
                            inside_drawable_text,
                            destroy_drawable_text);
}
