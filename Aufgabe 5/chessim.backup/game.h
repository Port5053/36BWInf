#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stddef.h>

enum Dir {RIGHT, UP, LEFT, DOWN};
enum Type {EMPTY, PAWN};

typedef int8_t Cord;

typedef struct Tile {
	Cord x;
	Cord y;
	Type type;
	bool visited;
	struct Tile **pawnEntry;
} Tile;

typedef struct {
	Tile *start;
	Tile *end;
} Move;

const int8_t DIM = 8;
const int8_t PAWN_N = 8;
const Dir headings[] = {RIGHT, UP, LEFT, DOWN};

const unsigned int DEPTH = 8;

extern Tile board[DIM][DIM], *pawns[PAWN_N];
extern Cord rookX, rookY;

void initBoard();
void putPawns(const Cord coords[PAWN_N][2]);
void putRook(Cord x, Cord y);
void clearVisited();

bool inBounds(Cord x, Cord y);
Tile* relativePos(const Tile& start, Dir heading, Cord length);

Move* possiblePawnMoves(Move* buffer);
Tile** nextRookMoves(const Tile &start, Tile **buffer);
void updateMovePossibilities(bool poss[DIM][DIM]);

bool isGood(const Tile& tile);
bool isGood(Cord x, Cord y);
unsigned int countAccessibleTiles(Tile& tile);

void doPawnMove(const Move& move);
void undoPawnMove(const Move& move);
void doRookMove(const Move& move);
void undoRookMove(const Move& move);

unsigned int evaluate();
unsigned int minPawns(unsigned int depth, unsigned int alpha, unsigned int beta);
unsigned int maxRook(unsigned int depth, unsigned int alpha, unsigned int beta);
Move getBestPawnMove();

#endif