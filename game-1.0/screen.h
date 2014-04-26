#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/fb.h>
#include <sys/mman.h>

#include "playgrid.h"
#define SCREENSIZE (320*240)
void init_screen();
void clean_screen();

void screen_tests();
void print_playgrid(PlayGrid grid);

#endif

