/* 36. BWInf Aufgabe 5 - Bauernopfer
   Kamal Abdellatif
*/

#include "game.h"

// Einbinden je nach Terminal oder grafischer Darstellung
#ifdef GUI
#	include "draw.h"
#else
#	include "textInterface.h"
#endif

unsigned int score, minScore;
Move buffer[32], *move, *minMove;

void react() {
	/* Macht den naechsten Bauernzug nach Strategie */
	possiblePawnMoves(buffer);  // Einlesen aller moeglichen Zuege
	minScore = 10000;

	// Ermittlung des Zugs der die Moeglichkeiten minimiert
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
	const Cord coords[][2] = {  // Startstellung
		{0, 2},
		{1, 5},
		{2, 2},
		{3, 5},
		{4, 2},
		{5, 5},
		{6, 2},
		{7, 5},
	};
	putPawns(coords);  // Setzen der Bauernstellung

	rookX = 0;  // Dummy-Coordinaten des Turms
	rookY = 0;

	bool first = true; // Erster Zug. Ist relevant fuer die Zugmoeglichkeiten.

#ifdef GUI
	do {
		setRookOnClick(first);
		react();
		if (first)
			first = false;
	} while (!userInterrupt());
#else
	while (1) {
		scanRook(first);
		react();
		printMove(*minMove);
		if (first)
			first = false;
	}
#endif
	return 0;
}

