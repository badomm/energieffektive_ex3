#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

#include "query_ioctl.h"
static i = 0;
void gamepad_handler(int signum){
	if(signum == SIGIO)
	{
		i++;
		printf("Got Signal%d\n",i);
	}
	signal(SIGIO, &gamepad_handler);
}

int main(void){
	char *filename = "/dev/gamepad_device";
	int fd = 0;
	while(fd <= 0){
		fd = open(filename,O_RDONLY);
	}
	while(1){
		signal(SIGIO, &gamepad_handler);
		query_gamepad_t q = {.led = 0, .buttons = 0};

		printf("%d\n",fd);
		if(ioctl(fd, WRITE2GAMEPAD, &q)==-1)
			printf("error\n");
		fflush(stdout);
	}
	close(fd);
}