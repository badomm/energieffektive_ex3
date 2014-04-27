#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>

typedef struct
{
	int led;
	int buttons;
} query_gamepad_t;

#define IOCTL_APP_TYPE 77

//http://www.circlemud.org/jelson/software/fusd/docs/node31.html
#define WRITE2GAMEPAD _IOW(IOCTL_APP_TYPE, 2, query_gamepad_t) 
#define READ_GAMEPAD _IOR(IOCTL_APP_TYPE,3,query_gamepad_t)

#endif

