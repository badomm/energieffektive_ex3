#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O3 -g -Wall -Wsign-compare -Wfloat-equal -Wformat-security -Werror
CPPFLAGS +=
LDFLAGS +=


all: game

game: screen.o playgrid.o sokoban.o game.o levels.o

clean:
	-rm -f game
	-rm -f game.gdb

install:


.PHONY: all install clean

# end of development makefile

