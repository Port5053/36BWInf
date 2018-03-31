#include "game.h"

Tile board[DIM][DIM], *pawns[PAWN_N];
Cord rookX, rookY;
Move bestPawnMove;

void initBoard() {
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			board[x][y]= {x, y, EMPTY, false, NULL};
}

void putPawns(const Cord coordinates[PAWN_N][2]) {
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
	rookX = tile.x;
	rookY = tile.y;
}

void clearVisited() {
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			board[x][y].visited = false;
}

bool inBounds(Cord x, Cord y) {
	return (0 <= x && x < DIM && 0 <= y && y < DIM);
}

Tile* relativePos(const Tile& start, Dir heading, Cord length) {
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
	if (!inBounds(x, y)) return false;
	return isGood(board[x][y]);
}

unsigned int countAccessibleTiles(Tile& tile) {
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
		// printf("found move %d %d -> %d %d\n", move->start->x, move->start->y, move->end->x, move->end->y);
		move++;
	}
	move->start = move->end = NULL;
	return buffer;
}
/*
unsigned int maxRook(unsigned int depth, unsigned int alpha, unsigned int beta) {
	if (depth == 0) {
		return evaluate();
	}

	unsigned int value;
	unsigned int maxValue = alpha;

	Move moves[2*DIM]; 
	// printf("testing moves\n");
	for (Move *move = possibleRookMoves(moves); move->start != NULL; move++) {
		doRookMove(*move);
		value = minPawns(depth-1, maxValue, beta);
		undoRookMove(*move);
		if (value > maxValue) {
			maxValue = value;
			if (maxValue >= beta)
				break;
		}
	}
	return maxValue;
}

unsigned int minPawns(unsigned int depth, unsigned int alpha, unsigned int beta) {
	if (depth == 0) {
		return evaluate();
	}

	unsigned int value, before;
	unsigned int minValue = beta;

	Move moves[4*PAWN_N];
	for (Move *move = possiblePawnMoves(moves); move->start != NULL; move++) {
		// printf("inloop\n");
		before = evaluate();
		doPawnMove(*move);
		if (before < evaluate()) {
			undoPawnMove(*move);
			continue;
		}
		value = maxRook(depth-1, alpha, minValue);
		undoPawnMove(*move);
		if (value < minValue) {
			minValue = value;
			if (minValue <= alpha)
				break;

			if (depth == DEPTH) {
				bestPawnMove = *move;
				printf("Best move %d %d %d %d\n", move->start->x, move->start->y, move->end->x, move->end->y);
			}
		}
	}
	return minValue;
}

Move getBestPawnMove() {
	minPawns(DEPTH, 0, DIM*DIM);
	return bestPawnMove;
}*/