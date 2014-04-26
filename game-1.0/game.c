#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "query_ioctl.h"
#include "screen.h"
#include "sokoban.h"

#define GAMEPAD_PATH "/dev/gamepad_device"

static int gamepad_input = 0;
static int gamepad_filID = 0;
void gamepad_handler(int signum){
	read(gamepad_filID,&gamepad_input,1);
	signal(SIGIO, &gamepad_handler);
}

int init_gamepad(){
	gamepad_filID = open(GAMEPAD_PATH,O_RDWR);
	if(gamepad_filID <= 0){
		printf("trying to open failed:");
		fflush(stdout);	
		return -1;
	}
	signal(SIGIO, &gamepad_handler);
	return 1;
}
void close_gamepad(){
	close(gamepad_filID);
}
int do_button_action(PlayGrid *grid){
	int end = 1;		
	switch(gamepad_input)
	{
		case 254: //sw1 -left
			sokoban_update(LEFT,grid);
			printf("Gamepad input     : %d\n",gamepad_input);
		break;
		case 253: //sw2 - up
			sokoban_update(UP,grid);
		break;
		case 251: //sw3 
			sokoban_update(RIGHT,grid);
		break;
		case 247: //sw4
			sokoban_update(DOWN,grid);
		break;
		case 239:  //sw5 Reset
			delete_sokoban(grid);
			grid = init_sokoban();
			grid2screen(grid);
		break;
		case 223:  //sw6 stop game
			end = 0;
		break;
		case 191: //sw7
		break;
		case 127:  //sw8
		break;
		default:
		break;
	}
	gamepad_input = 255;
	return end;
}



int main(void) {
	printf("Gaming time!\n");
	
	PlayGrid* grid = init_sokoban();
	if(!init_gamepad()) exit(-1);	
	init_screen();
	grid2screen(grid);
	
	while(do_button_action(grid) != 0){
		pause();
	}
	
	delete_sokoban(grid);
	close_gamepad();
	fill_screen(0x0000);
	clean_screen();
	
	return 0;
}


