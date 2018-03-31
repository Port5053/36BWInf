from time import clock
from PIL import Image, ImageDraw

size = 50  # px

BLACK = 0x000000
WHITE = 0xFFFFFF


class Wall:
	def __init__(self, n):
		self.n = n
		self.width = n * (n + 1) // 2
		self.height = n // 2 + 1

		self.slits = [True for _ in range(self.width - 1)]
		self.rows = [[None for _ in range(n)] for _ in range(self.height)]
		self.open_gaps = [n for _ in range(self.height)]
		self._row_i = 0

	@property
	def lengths(self):
		return range(1, self.n + 1)

	@property
	def row(self):
		return self.rows[self._row_i]

	def free_slit(self, pos, offset=0):
		slit = pos + offset - 1
		if slit < -1 or slit > self.width - 1:
			return False

		elif slit == -1 or slit == self.width - 1:
			return True

		return self.slits[slit]

	def overlap(self, pos, length):
		for p1, l1 in zip(self.row, self.lengths):
			if p1 is None:
				continue

			if p1 < pos < p1+l1 or pos < p1 < pos+length:
				print("Overlap", pos, length, p1, l1)
				return True

		return False

	def free(self, pos, length):
		return self.free_slit(pos) \
			and self.free_slit(pos, length) \
			and not self.overlap(pos, length) 


	def set_slit(self, pos, offset=0, value=False):
		slit = pos + offset - 1
		if 0 <= slit < self.width - 1:
			self.slits[slit] = value

	def put_brick(self, pos, length):
		if self.open_gaps[self._row_i] == 0:
			self._row_i += 1
		self.open_gaps[self._row_i] -= 1
		self.row[length-1] = pos
		self.set_slit(pos, value=False)
		self.set_slit(pos, length, value=False)

	def pop_brick(self, length):
		self.open_gaps[self._row_i] += 1
		pos = self.row[length-1]
		self.row[length-1] = None
		self.set_slit(pos, value=True)
		self.set_slit(pos, length, value=True)
		if self.open_gaps[self._row_i] == self.n:
			self._row_i -= 1

	def possible_places(self, length):
		for pos in range(self.width - length + 1):
			if self.free(pos, length):
				yield pos

	def iterate_bricks(self):
		lengths = []
		for l in self.lengths:
			if self.row[l-1] is None:
				lengths.append(l)

		options = lambda length: sum(1 for _ in self.possible_places(length))
		lengths.sort(key=options)

		for length in lengths:
			places = tuple(self.possible_places(length))
			if not places:
				return

			yield length, places

	def _get_image(self):
		image = Image.new("RGB", (size*self.width, size*self.height), WHITE)
		draw = ImageDraw.Draw(image)
		for y, row in enumerate(self.rows):
			for x, length in zip(row, self.lengths):
				if x is not None:
					draw.rectangle((x*size, y*size, (x+length)*size, (y+1)*size), outline=BLACK)

		return image

	def save(self, filename):
		self._get_image().save(filename)

	def show(self):
		self._get_image().show()


def solve(n):
	wall = Wall(n)
	depth = 0
	max_depth = wall.height * n

	def push_brick(pos=None, length=None, first=False):
		nonlocal depth
		print("Enter", pos, length)
		if not first:
			wall.put_brick(pos, length)
			depth += 1

			if depth == max_depth:
				return True

		for l, places in wall.iterate_bricks():
			for place in places:
				if push_brick(place, l):
					return True

		if not first:
			wall.pop_brick(length)
			depth -= 1

		print("exit")
		return False

	if push_brick(first=True):
		return wall

	return None
	   

wall = solve(2)

# wall.put_brick(0, 1)
# wall.put_brick(1, 2)
# wall.put_brick(3, 3)
# wall.put_brick(6, 4)
# wall.put_brick(0, 2)
# wall.put_brick(2, 3)
# wall.put_brick(5, 4)
# wall.put_brick(9, 1)
# for l, p in wall.iterate_bricks():
	# print(l, p)
if wall:
	wall.show()

else:
	print("no solution")
