#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/fb.h>

#include <sys/mman.h>
#define SCREENSIZE (320*240)

void screen_tests();

#endif

