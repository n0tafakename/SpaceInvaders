CFLAGS = -g -Wall -Wextra -Og -std=c99 -pedantic -Wno-gnu-binary-literal

si:
	gcc 8080.c 8080.h SpaceInvaders.h SpaceInvaders.c main.c -I include -L lib -l SDL2-2.0.0
