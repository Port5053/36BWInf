#include <stdio.h>
#include "game.h"

void scanRook() {
	Cord x, y;
	char file, rank, nl;
	fflush(stdout);
	scanf("%c%c%c", &file, &rank, &nl);
	x = file - 'a';
	y = rank - '1';
	if (inBounds(x, y)) { // todo is possible rook move
		printf("ok ");
		rookX = x;
		rookY = y;
	} else
		scanRook();	
}

void printMove(const Move& move) {
	printf("%c%c-%c%c\n", 'a'+move.start->x, '1'+move.start->y, 'a'+move.end->x, '1'+move.end->y);
}