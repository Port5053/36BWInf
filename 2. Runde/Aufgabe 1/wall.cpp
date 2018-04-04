/**
*	36. BWInf 1. Aufgabe
*	Die Kunst der Fuge
*	Kamal Abdellatif
*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>

#ifdef IMAGE  // Makro fuer Bildausgabe
#	include "CImg.h"
#	ifndef GUI
#		include <string>
#	endif
using namespace cimg_library;
const int IMG_WIDTH = 1200;
const int TILE_SIZE = 50;
const unsigned char FOREGROUND[] = {255, 255, 255};
#endif

class Wall {
public:
	Wall(int n);
	bool valid(const int& row, const int& skip) const;
	bool finished() const;
	int fill(const int& row, const int& skip);
	void pop(const int& row, const int& brick, const int& skip);
#ifdef IMAGE
	CImg<unsigned char> getImage() const;
#endif
	void printWall() const;
	int n,
		width,  // Breite w
		height,  // Hoehe h
		slit,  // aktuelle Spalte s
		spaces;  // r, dargestellt als Rest von Division

	// 2D-Array fuer benutzte Steine (siehe Sektion 3.1)
	std::vector< std::vector<bool> > usedBricks;

	// Array fuer Laengen der Reihen nach Indices
	std::vector<int> endpoints;

	// Linked-List fuer Reihenfolge der Reihen
	std::list<int> order;

	// Speicher fuer Loesungen beim Backtracking
	std::forward_list< std::pair<int, int> > solution;
	// jedes Pair (i, z) besteht aus dem Index i der
	// gefuellten Reihe i und den uebersprungenen Spalten z
};

Wall::Wall(int _n) {
	/* Initialisiert eine Wand n mit allen Datenstrukturen*/
	n = _n;
	width = n*(n+1)/2;  
	height = n/2 + 1;  // C++ uebernimmt floor durch Integer-Division
	spaces = (width-1) % (n-1);  

	usedBricks = std::vector< std::vector<bool> >(
		height, std::vector<bool>(n, false));

	// Laengen werden mit 0 initialisiert
	endpoints = std::vector<int>(height, 0);

	// Aufbau der aufsteigenden Reihenfolge
	for (int i = 0; i < height; ++i)
		order.push_back(i);

	solution = std::forward_list< std::pair<int, int> >();
	slit = 0;
}

bool Wall::valid(const int& row, const int& skip) const {
	/* Gibt zurueck ob der Schritt vom Anlegen in Reihe row mit
	   skip Spruengen moeglich ist. */
	int brick = slit + 1 + skip - endpoints.at(row);
	if (brick > n)  // Stein zu lang
		return false;
	if (usedBricks.at(row).at(brick-1))  // Stein schon enthalten
		return false;
	return true;
}

int Wall::fill(const int& row, const int& skip) {
	/* Fuegt einen Stein in die Reihe row mit Uberspringen
	   von skip Spalten an und aktualisiert die Laengen
	   und Steinbelegungen. Gibt die Laenge b des
	   Steins zurueck. */
	int brick = slit + 1 + skip - endpoints.at(row);
	usedBricks.at(row).at(brick-1) = true;
	endpoints.at(row) += brick;
	slit += (1+skip);
	spaces -= skip;
	return brick;
}

void Wall::pop(const int &row, const int& brick, const int& skip) {
	/* Macht den letzten Schritt des Anfuegens des Steins brick an
	   row mit ueberspringen von skip Spalten rueckgaenig. */
	usedBricks.at(row).at(brick-1) = false;
	endpoints.at(row) -= brick;
	slit -= (1+skip);
	spaces += skip;
}

bool Wall::finished() const {
	/* Gibt zurueck, ob die Wand vollstaendig gefuellt ist. */
	return (slit == width);
}
#ifdef IMAGE
CImg<unsigned char> Wall::getImage() const {
	/* Ausgabe der Wand als Bild */
	int size = std::min(TILE_SIZE, IMG_WIDTH/(int) width);
	CImg<unsigned char> image(width*size+1, height*size+1, 1, 1, 0);

	std::vector<int> xpos(height, 0);
	int slit = 1;
	for (const auto& pair : solution) {
		image.draw_rectangle(size*xpos.at(pair.first)+1, size*pair.first+1,
			size*slit-1, size*(pair.first+1)-1, FOREGROUND);
		xpos.at(pair.first) = slit;
		slit += (1+pair.second);
	}
	for (int y = 0; y < height; ++y) {
		if (xpos.at(y) < width)
			image.draw_rectangle(size*xpos.at(y)+1, size*y+1,
				size*width-1, size*(y+1)-1, FOREGROUND);
	}

	return image;
}
#endif

void Wall::printWall() const {
	/* Ausgabe der Wand als Text */
	int end, slit;
	for (int row = 0; row < height; ++row) {
		end = 0;
		slit = 1;
		for (auto pair : solution) {
			if (pair.first == row) {
				std::cout << slit - end << " ";
				end = slit;
			}
			slit += (1+pair.second);
		}
		if (end < width)
			std::cout << width - end << std::endl;
		else
			std::cout << std::endl;
	}
}

bool pushSlit(Wall& wall, const int& row, const int& skip, bool first) {
	/* Rekursive Funktion. Fuegt einen Stein an die Reihe mit Index row
	   an und ueberspringt skip Spalten. Danach wird jede Folgemoeglichkeit
	   eines Weiteren Steins getestet. Ist die Mauer wall dadurch gefuellt
	   worden oder einer der Moeglichkeiten war selbst erfolgreich, wird wahr
	   zurueckgegeben. Andernfalls werden die Aenderungen rueckgaenig gemacht
	   und Fehlschlag zurueckgegeben. */

	int brick, nextRow;
	if (!first)
		brick = wall.fill(row, skip);  // Fuellen der Reihe
	if (wall.finished())  // Abbruchbedingung
		return true;

	std::list<int>::const_iterator next, place;  // Pointer in Linked-List

	// Iteration durch alle Sprunglaengen z (nextSkip)
	for (int nextSkip = 0; nextSkip <= wall.spaces; ++nextSkip) {

		// Iteration durch alle Reihen (next) nach Reihenfolge
		for (next = wall.order.begin(); next != wall.order.end(); ++next) {
			if (wall.valid(*next, nextSkip)) {
				nextRow = *next;
				place = wall.order.erase(next);  // Entfernen von R 
				wall.order.push_back(nextRow);  // Anfuegen ans Ende

				// Rekursiver Funktionsaufruf
				if (pushSlit(wall, nextRow, nextSkip, false)) {
					wall.solution.emplace_front(  // Anfuegen an Loesung
						std::make_pair(nextRow, nextSkip));
					return true;  // Erfolg
				}
				wall.order.pop_back();  // Wiederherstellung der Reihenfole
				wall.order.insert(place, nextRow);
			}
		}
	}
	if (!first)
		wall.pop(row, brick, skip);  // Rueckgaenigmachen der Aenderungen
	return false;  // Fehlschlag
}

bool solve(Wall& wall) {
	/* Initialisierender Funktionsaufruf fuer Rekursion */
	return (pushSlit(wall, 0, 0, true));
}

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		std::cerr << "wall: too few arguments" << std::endl;
		return 1;
	}
	int n = std::atoi(argv[1]);
	Wall wall(n);
	solve(wall);
#ifdef IMAGE
	auto image = wall.getImage();
#ifdef GUI
	CImgDisplay disp(image);
	image.display(disp);
	while (!disp.is_closed() && !disp.is_keyESC())
		disp.wait();
#else
	image.save((std::to_string(n) + ".png").c_str());
#endif // GUI
#else
	wall.printWall();
#endif  // IMAGE
	return 0;
}