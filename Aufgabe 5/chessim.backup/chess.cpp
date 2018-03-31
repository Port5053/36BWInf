#include "game.h"

#ifdef GUI
#	include "draw.h"
#else
#	include "textInterface.h"
#endif

unsigned int score, minScore;
Move buffer[32], *move, *minMove;

void react() {
	possiblePawnMoves(buffer);
	minScore = 10000;
	for (move = &buffer[0]; move->start != NULL; move++) {
		doPawnMove(*move);
		score = evaluate();
		if (score < minScore) {
			minScore = score;
			minMove = move;
		}
		undoPawnMove(*move);
	}
	doPawnMove(*minMove);
}

int main(int argc, char const *argv[]) {
	initBoard();
	const Cord coords[][2] = {
		{0, 2},
		{1, 5},
		{2, 2},
		{3, 5},
		{4, 2},
		{5, 5},
		{6, 2},
		{7, 5},
	};
	putPawns(coords);

	rookX = 0;
	rookY = 0;


	bool first = true;

	#ifdef GUI
	do {
		setRookOnClick(first);
		react();
		if (first)
			first = false;
	} while (!userInterrupt());
	#else
	while (1) {
		scanRook();
		react();
		printMove(*minMove);
	}
	#endif
	return 0;
}

