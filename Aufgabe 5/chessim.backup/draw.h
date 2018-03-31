#ifndef DRAW_H
#define DRAW_H

#include <time.h>
#include "CImg.h"
#include "game.h"

using namespace cimg_library;

const unsigned int TILE_SIZE = 60;
const unsigned int WIDTH = 8*TILE_SIZE, HEIGHT = 8*TILE_SIZE;
const bool fullscreen = false;

const float COOLDOWN = .1;  // seconds?

void getMousePos(Cord &x, Cord &y);
void drawGrid();
void draw(bool showRook);
void drawSelected();
void setRookOnClick(bool showRook);
bool userInterrupt();

#endif