/* 36. BWInf Aufgabe 5 - Bauernopfer
   Kamal Abdellatif
*/

#include <stdio.h>
#include "game.h"

bool movePoss[DIM][DIM];

void scanRook(bool isFirst) {
	/* Liest aus stdin die naechste Position fuer den Turm in rookX, rookY ein */
	updateMovePossibilities(movePoss);
	Cord x, y;
	char file, rank;
	if (!isFirst)
		printf("%c%c-", 'a'+rookX, '1'+rookY);  // ausgeben der aktuellen Position

	scanf(" %c%c", &file, &rank);
	x = file - 'a';  // char operation magic
	y = rank - '1';
	if ((isGood(x, y) && isFirst) || (movePoss[x][y] && !isFirst)) {
		printf("ok ");
		rookX = x;
		rookY = y;
	} else
		scanRook(isFirst);	
}

void printMove(const Move& move) {
	/* Gibt einen Bauernzug move auf stdout aus */
	printf("B%c%c-%c%c\n", 'a'+move.start->x, '1'+move.start->y, 'a'+move.end->x, '1'+move.end->y);
}