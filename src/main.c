#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "util.h"

/* Negative Nancy's Video Studio
 * 
 * Concepts:
 * 
 * The _stage_ is the state machine representing what is happening on screen
 * at any given time. This includes audio and visual information.
 *
 * The _director_ is the mechanism that updates the stage over time according
 * to a script. */

/* TODO:
 * instead of selecting a mode, there are three output modes, and any
 * cobination of them can be selected:
 * - realtime playback in a window mode, 
 * - encode to a video file mode,
 * - stream to video server mode
 *
 * Also, there will be two of these modes to be chosen from:
 * - interactive mode, without a script
 * - script mode, which interprets and executuse from a script
 */

/* program use mode */
typedef enum app_mode_t {

    REALTIME, /* preview the video real time via the window manager */
    EXPORT,   /* render and export the video to a video file */
    TEST      /* run tests to verify proper functioning */

} app_mode_t;

/* external entry points */
int entry_realtime (char *script_path);
int entry_export (char *script_path, char *export_path);
int entry_test ();

/* print command line usage */
void usage (FILE *stream, char *command) {

    fprintf (stream, "Usage: %s [-hDT] [-o output.mp4] [script.vs]\n", command);
    fputs ("Options:\n",                                                   stream); 
    fputs ("\t-o output.mp4  render to video file instead of on screen\n", stream);
    fputs ("\t-h             show this usage\n",                           stream);
    fputs ("\t-D             enable debug features\n",                     stream);
    fputs ("\t-T             run tests\n",                                 stream);
}

/* main entry point */
int main (int argc, char **argv) {

    app_mode_t app_mode = REALTIME;
    char *export_path = NULL;
    char *script_path = NULL;
    int opt;

    puts ("Hewwo!!!:'3");

    /* parse commad line arguments */
    while ((opt = getopt (argc, argv, "ho:DT")) != -1) {
        
        switch (opt) {

            case 'h':
                usage (stdout, argv[0]);
                exit (EXIT_SUCCESS);
                
            case 'o':
                app_mode = EXPORT;
                export_path = optarg;
                break;

            case 'D':
                DEBUG = true;
                break;

            case 'T':
                app_mode = TEST;
                break;

            default:
                usage (stderr, argv[0]);
                exit (EXIT_FAILURE);
        }
    }

    /* get the script file to read if given */
    if (optind < argc)
        script_path = argv[optind];

    /* debug info */
    if (DEBUG) {

        char *mode_string;
        switch (app_mode) {

            case REALTIME:
                mode_string = "realtime";
                break;

            case EXPORT:
                mode_string = "export";
                break;

            case TEST:
                mode_string = "test";
                break;
        }

        _debug_printf ("Debug mode on!!!\n");
        _debug_printf ("App mode: [%s]\n", mode_string);
        _debug_printf ("Export path: %s\n", export_path);
        _debug_printf ("Script path: %s\n", script_path);
    }

    /* branch to entry point for the selected app mode */
    switch (app_mode) {

        case REALTIME:
            return entry_realtime (script_path);

        case EXPORT:
            return entry_export (script_path, export_path);

        case TEST:
            return entry_test ();
    }

    return EXIT_SUCCESS;
}
