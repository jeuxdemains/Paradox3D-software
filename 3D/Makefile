LIB=/usr/local/Cellar/sdl2/2.0.14_1/lib
INCLUDE=/usr/local/Cellar/sdl2/2.0.14_1/include

all:
	gcc *.c -o p3d -lSDL2 -lm -O3

mac:
	gcc *.c -o p3d -lSDL2 -L$(LIB) -I$(INCLUDE) -lm -O3
	
debug:
	gcc *.c -o p3d -lSDL2 -lm -g
