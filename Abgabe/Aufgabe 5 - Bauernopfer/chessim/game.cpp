/* 36. BWInf Aufgabe 5 - Bauernopfer
   Kamal Abdellatif
*/

#include "game.h"

Tile board[DIM][DIM], *pawns[PAWN_N];
Cord rookX, rookY;
Move bestPawnMove;

void initBoard() {
	/* Initialisierung des Bretts (board) */
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			board[x][y]= {x, y, EMPTY, false, NULL};
}

void putPawns(const Cord coordinates[PAWN_N][2]) {
	/* Besetzt board mit Bauern aus den Koordinatenpaaren coordinates */
	Cord x, y;
	for (int i = 0; i < PAWN_N; ++i) {
		x = coordinates[i][0];
		y = coordinates[i][1];
		board[x][y].type = PAWN;
		pawns[i] = &board[x][y];
		board[x][y].pawnEntry = &pawns[i];
	}
}

void putRook(const Tile& tile) {
	/* Setzt den Turm auf ein Feld tile */
	rookX = tile.x;
	rookY = tile.y;
}

void clearVisited() {
	/* Setzt den visited-Flag fuer die Tiefensuche zurueck */
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			board[x][y].visited = false;
}

bool inBounds(Cord x, Cord y) {
	/* Gibt zurueck, ob ein Punkt (x, y) sich auf dem Brett befindet */
	return (0 <= x && x < DIM && 0 <= y && y < DIM);
}

Tile* relativePos(const Tile& start, Dir heading, Cord length) {
	/* Gibt die Position zurueck, welche length Felder von start in
	Richtung heading liegt */
	Cord x = start.x, y = start.y;
	switch (heading) {
		case RIGHT: x += length;
					break;
		case UP:    y += length;
					break;
		case LEFT:  x -= length;
					break;
		case DOWN:  y -= length;
					break;
	}
	if (!inBounds(x, y)) return NULL;
	return &board[x][y];

} 

Tile** nextRookMoves(Tile& start, Tile **buffer) {
	/* Beschreibt **buffer mit Pointern auf alle Felder, die der Turm 
	im naechsten Zug erreichen kann */
	bool ended[] = {false, false, false, false};

	Tile *t;
	Tile **next = buffer;
	*(next++) = &start;
	for (Cord length = 1; length < DIM; ++length) {
		for (Dir heading : headings) {
			if (ended[heading])
				continue;
			t = relativePos(start, heading, length);
			if (t == NULL || t->type == PAWN)
				ended[heading] = true;
			else if (isGood(*t))
				*(next++) = t;
		}
	}
	*next = NULL;
	return buffer;
}

void updateMovePossibilities(bool poss[DIM][DIM]) {
	/* Beschreibt den Buffer poss mit Wahrheits werden.
	poss[x][y] == true wenn der Turm das Feld (x, y) in einem Zug
	erreichen kann, sonst false. */
	Tile *buffer[2*DIM];
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			poss[x][y] = false;

	nextRookMoves(board[rookX][rookY], buffer);
	for (Tile **next = &buffer[0]; *next != NULL; next++) {
		poss[(*next)->x][(*next)->y] = true;
	}
}

bool isGood(const Tile& tile) {
	/* Gibt zurueck, ob das Feld tile unbedroht ist. */
	if (tile.type == PAWN) return false;
	Tile *neighbor;
	for (Dir heading : headings) {
		neighbor = relativePos(tile, heading, 1);
		if (neighbor == NULL)
			continue;
		if (neighbor->type == PAWN)
			return false;
	}
	return true;
}

bool isGood(Cord x, Cord y) {
	/* Gibt zurueck, ob das Feld (x, y) unbedroht ist. */
	if (!inBounds(x, y)) return false;
	return isGood(board[x][y]);
}

unsigned int countAccessibleTiles(Tile& tile) {
	/* Rekursive Funktion, die die Anzahl der erreichbaren
	Felder bestimmt */
	tile.visited = true;
	unsigned int count = 0;
	if (isGood(tile))
		count ++;
	Tile* buffer[2*DIM];
	nextRookMoves(tile, buffer);
	for (Tile **next = &buffer[0]; *next != NULL; next++)
		if (!(*next)->visited)
			count += countAccessibleTiles(**next);

	return count;
}

unsigned int evaluate() {
	/* Gibt die Anzahl von Zugmoeglichkeiten fuer den Turm zurueck */
	unsigned int count = countAccessibleTiles(board[rookX][rookY]);
	clearVisited();
	return count;
}

void doPawnMove(const Move& move) {
	move.start->type = EMPTY;
	move.end->type = PAWN;
	*(move.start->pawnEntry) = move.end;
	move.end->pawnEntry = move.start->pawnEntry;
	move.start->pawnEntry = NULL;
}

void undoPawnMove(const Move& move) {
	Move reverse = {move.end, move.start};
	doPawnMove(reverse);
}

void doRookMove(const Move& move) {
	rookX = move.end->x;
	rookY = move.end->y;
}

void undoRookMove(const Move& move) {
	rookX = move.start->x;
	rookY = move.start->y;
}

Move* possiblePawnMoves(Move *buffer) {
	Tile *end;
	Move *move = buffer;
	for (Tile *start : pawns) {
		for (Dir heading : headings) {
			end = relativePos(*start, heading, 1);
			if (end == NULL)
				continue;
			if (end->type == EMPTY) {
				move->start = start;
				move->end = end;
				move++;
			}
		}
	}
	move->start = move->end = NULL;
	return buffer;
}

Move* possibleRookMoves(Move *buffer) {
	Tile *start = &board[rookX][rookY];
	Tile *ends[16];
	Move *move = buffer;
	for (Tile **end = nextRookMoves(*start, ends); *end != NULL; end++) {
		move->start = start;
		move->end = *end;
		move++;
	}
	move->start = move->end = NULL;
	return buffer;
}