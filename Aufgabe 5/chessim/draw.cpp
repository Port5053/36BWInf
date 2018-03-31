/* 36. BWInf Aufgabe 5 - Bauernopfer
   Kamal Abdellatif
*/

#include "draw.h"

const unsigned char WHITE[] = {255};

CImg<unsigned char> image(WIDTH, HEIGHT, 1, 1),
                    empty(TILE_SIZE, TILE_SIZE, 1, 1, 0),
                    selected(TILE_SIZE, TILE_SIZE, 1, 1, 60),
                    rook("res/tower.png"),
                    pawn("res/pawn.png");

CImgDisplay disp(image, "Chess", 1, fullscreen);

bool movePoss[DIM][DIM];
clock_t lastClick = clock();

void getMousePos(Cord &x, Cord &y) {
	x = disp.mouse_x()/TILE_SIZE;
	y = DIM - 1 - disp.mouse_y()/TILE_SIZE;
}

void drawGrid() {
	for (unsigned int i = 0; i < DIM; ++i)
		image.draw_line(TILE_SIZE*i, 0, TILE_SIZE*i, HEIGHT-1, WHITE);
	for (unsigned int i = 0; i < DIM; ++i)
		image.draw_line(0, TILE_SIZE*i, WIDTH-1, TILE_SIZE*i, WHITE);

	image.draw_line(WIDTH-1, 0, WIDTH-1, HEIGHT-1, WHITE);
	image.draw_line(0, HEIGHT-1, WIDTH-1, HEIGHT-1, WHITE);
}

void drawSelected(bool isFirst) {
	Cord x, y; getMousePos(x, y);
	if (!inBounds(x, y))
		return;

	if (movePoss[x][y] || (isFirst && isGood(x, y)))
		image.draw_image(x*TILE_SIZE, (DIM-1-y)*TILE_SIZE, selected);
}

void draw(bool isFirst) {
	for (Cord x = 0; x < DIM; ++x)
		for (Cord y = 0; y < DIM; ++y)
			image.draw_image(
				x*TILE_SIZE, (DIM-1-y)*TILE_SIZE,
				(board[x][y].type == PAWN) ? pawn : empty);

	drawSelected(isFirst);
	if (!isFirst)
		image.draw_image(rookX*TILE_SIZE, (DIM-1-rookY)*TILE_SIZE, rook);
	drawGrid();
	image.blur(.5).display(disp);	
}

void setRookOnClick(bool isFirst) {
	updateMovePossibilities(movePoss);
	while (!disp.button()&1 || (float)(clock()-lastClick)/CLOCKS_PER_SEC < COOLDOWN) {
		draw(isFirst);
		disp.wait();
		if (userInterrupt())
			return;
	}  // left mouse button
	lastClick = clock();
	Cord x, y; getMousePos(x, y);
	if (movePoss[x][y] || (isFirst && isGood(x, y))) {  // erstes mal
		rookX = x;
		rookY = y;
	}
	else setRookOnClick(isFirst);
}

bool userInterrupt() {
   	return (disp.is_closed() || disp.is_keyESC());
} 