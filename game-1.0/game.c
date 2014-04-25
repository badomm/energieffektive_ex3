#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "query_ioctl.h"
#include "screen.h"
static i = 0;

static int i = 0;
void gamepad_handler(int signum){
	printf("signum: %d\n",signum);
	i++;
	printf("Got Signal%d\n",i);
	signal(SIGIO, &gamepad_handler);
}

int main(void){
	char *filename = "/dev/gamepad_device";
	int fd = 0;
	printf("starting game!!!!!!\n");

	screen_tests();

	fflush(stdout);
	fd = open(filename,O_RDWR);
	printf("Trying to open file: %d\n",fd);
	fflush(stdout);
	if(fd <= 0){
		printf("trying to open failed: %d",fd);
		fflush(stdout);	
		return 0;
	}
	printf("file is open \n");

	while(1)
	{
		signal(SIGIO, &gamepad_handler);
		query_gamepad_t q = {.led = 0, .buttons = 0};

		//if(ioctl(fd, WRITE2GAMEPAD, &q)==-1)
			//printf("error\n");
		//sleep(1000);
		//printf("Staying alive\n");
	}
	close(fd);
}
