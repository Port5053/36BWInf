#!/usr/bin/python3.5

from time import clock
from PIL import Image, ImageDraw
import sys

n = int( sys.argv[1] )

IMAGE_WIDTH = 1300

BLACK = 0x000000
WHITE = 0xFFFFFF

class Wall:
	def __init__(self, n):
		self.n = n
		self.width = n*(n+1) // 2
		self.height = n//2 + 1
		self.spaces = (self.width-1) % (n-1)

		self.size = int(IMAGE_WIDTH / self.width)  # px

		self.rows = [[] for _ in range(self.height)]
		self.endpoints = [0 for _ in range(self.height)]
		self.current_slit = 0

	def valid_next(self, row, skip=0):
		length = self.current_slit + 1 + skip - self.endpoints[row]
		if length > self.n:
			return False

		if length in self.rows[row]:
			return False

		return True

	def sorted_rows(self):
		rows = list(range(self.height))
		rows.sort(key=lambda r: self.endpoints[r])
		return rows

	def fill_next(self, row, skip=0):
		length = self.current_slit + 1 + skip - self.endpoints[row]
		self.rows[row].append(length)
		self.endpoints[row] += length
		self.current_slit += 1+skip
		self.spaces -= skip

	def pop(self, row, skip=0):
		self.endpoints[row] -= self.rows[row].pop()
		self.current_slit -= (1+skip)
		self.spaces += skip

	def finished(self):
		return self.current_slit == self.width-1

	def fill_remaining(self):
		for row in range(self.height):
			self.rows[row].append(self.width-self.endpoints[row])

	def _get_image(self):
		image = Image.new("RGB", (self.size*self.width, self.size*self.height), WHITE)
		draw = ImageDraw.Draw(image)
		for y, row in enumerate(self.rows):
			x = 0
			for l in row:
				draw.rectangle((x*self.size, y*self.size, (x+l)*self.size, (y+1)*self.size), outline=BLACK)
				x += l

		return image

	def _get_tikz(self):
		command = ["\\draw[very	thick]"]
		for y, row in enumerate(self.rows):
			x = 0
			for l in row:
				command.append( "({}, {}) rectangle ({}, {})".format(x, y, x+l, y+1) )
				x += l

		return " ".join(command) + ";"

	def save(self, filename):
		self._get_image().save(filename)

	def show(self):
		self._get_image().show()


wall = Wall(n)

def push_slit(row=None, skip=0, init=False):
	if not init:
		wall.fill_next(row, skip)
		if wall.finished():
			return True

	_sorted_rows = wall.sorted_rows()
	print(*_sorted_rows, end=' \n')
	for next_skip in range(wall.spaces+1):
		for next_row in _sorted_rows:
			if wall.valid_next(next_row, next_skip):
				if push_slit(next_row, next_skip):
					return True

	if not init:
		wall.pop(row, skip)
	
	return False

start = clock()
result = push_slit(init=True)
dt = clock() - start

wall.fill_remaining()

# print("Done in {:.6f}s".format(dt))

# print(wall._get_tikz())
wall.show()
