# Negative Nancy's Video Studio

Negative Nancy's live video production software!

It's the software Negative Nancy uses to create her Youtube videos and streams!

_NOTE: this is super ultra pre-alpha right now!! none of this is reliable_

_TODO: script and gif example_

## Dependencies

You'll need... _TODO_

## Build and Install

Building produces the binary `build/nnvs` which is the video compiler!
To simply build it, just `cd` to the root of the repository and do this:

```bash
make
```

To build and run without installation (e.g. if you want to work on the code and test), run this, which will run it on the example video script:

```bash
make debug
```

Installation installs the binary and the man page.
To install it to your system, do this one:

```bash
sudo make install
```

To delete all the build files in the repository just do this:

```bash
make clean
```

## Quick Start

For a more complete manual and tutorials, see the wiki! _TODO: wiki, and link here_

For the complete reference of command line usage, see the man page at `man nnvs`!

First create a video script file named `my_script.vs` like this:

```
TODO
```

To preview this script before outputting it to a video, do this:

```bash
nnvs my_script.vs
```

When you like it and want to render it into a video, do this:

```bash
nnvs my_script.vs -o my_video.mp4
```
