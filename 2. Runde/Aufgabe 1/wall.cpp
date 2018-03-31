#include <algorithm>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>

#ifdef IMAGE
#include "CImg.h"
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
// private:
	int n, width, height, slit, spaces;

	std::vector< std::vector<bool> > usedBricks;
	std::vector<int> endpoints;
	std::list<int> order;
	std::forward_list< std::pair<int, int> > solution;
};

Wall::Wall(int _n) {
	n = _n;
	width = n*(n+1)/2;
	height = n/2 + 1;
	spaces = (width-1) % (n-1);
	usedBricks = std::vector< std::vector<bool> >(
		height, std::vector<bool>(n, false));
	endpoints = std::vector<int>(height, 0);
	for (int i = 0; i < height; ++i)
		order.push_back(i);
	solution = std::forward_list< std::pair<int, int> >();
	slit = 0;
}

bool Wall::valid(const int& row, const int& skip) const {
	int brick = slit + 1 + skip - endpoints.at(row);
	if (brick > n)
		return false;
	if (usedBricks.at(row).at(brick-1))
		return false;
	return true;
}

int Wall::fill(const int& row, const int& skip) {
	int brick = slit + 1 + skip - endpoints.at(row);
	usedBricks.at(row).at(brick-1) = true;
	endpoints.at(row) += brick;
	slit += (1+skip);
	spaces -= skip;
	return brick;
}

void Wall::pop(const int &row, const int& brick, const int& skip) {
	usedBricks.at(row).at(brick-1) = false;
	endpoints.at(row) -= brick;
	slit -= (1+skip);
	spaces += skip;
}

bool Wall::finished() const {
	return (slit == width);
}
#ifdef IMAGE
CImg<unsigned char> Wall::getImage() const {
	int size = std::min(TILE_SIZE, IMG_WIDTH/(int) width);
	CImg<unsigned char> image(width*size, height*size, 1, 1, 0);

	std::vector<int> xpos(height, 0);
	int slit = 1;
	for (const auto& pair : solution) {
		image.draw_rectangle(size*xpos.at(pair.first)+1, size*pair.first+1,
			size*slit-1, size*(pair.first+1)-1, FOREGROUND);
		xpos.at(pair.first) = slit;
		slit += (1+pair.second);
	}
	for (int y = 0; y < height; ++y)
		image.draw_rectangle(size*xpos.at(y)+1, size*y+1,
			size*width-1, size*(y+1)-1, FOREGROUND);

	return image;
}
#endif

void Wall::printWall() const {
	int end, s;
	for (int y = 0; y < height; ++y) {
		s = end = 0;
		for (const auto& pair : solution) {
			s += (1+pair.second);
			if (pair.first == y) {
				std::cout << s - end << " ";
				end = s;
			}
		}
		std::cout << width - end << std::endl;
	}
}

bool pushSlit(Wall& wall, const int& row, const int& skip, bool first) {
	int brick, nextRow;
	if (!first)
		brick = wall.fill(row, skip);
	if (wall.finished())
		return true;
	std::list<int>::const_iterator next, place;
	for (int nextSkip = 0; nextSkip <= wall.spaces; ++nextSkip) {
		for (next = wall.order.begin(); next != wall.order.end(); ++next) {
			if (wall.valid(*next, nextSkip)) {
				nextRow = *next;
				place = wall.order.erase(next);
				wall.order.push_back(nextRow);
				if (pushSlit(wall, nextRow, nextSkip, false)) {
					if (!first)
						wall.solution.push_front(
							std::make_pair(nextRow, nextSkip));
					return true;
				}
				wall.order.pop_back();
				wall.order.insert(place, nextRow);
			}
		}
	}
	if (!first)
		wall.pop(row, brick, skip);
	return false;
}

bool solve(Wall& wall) {
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
	CImgDisplay disp(image);
	image.display(disp);
	while (!disp.is_closed() && !disp.is_keyESC()) {
		disp.wait();
	}
#else
	wall.printWall();
#endif
	return 0;
}