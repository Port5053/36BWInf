import sys
from PIL import Image, ImageDraw

size = 50  # px

BLACK = 0x000000
WHITE = 0xFFFFFF

class Wall:
	def __init__(self, n):
		self.n = n
		self.width = n*(n+1)//2
		self.height = n//2 + 1
		self.rows = [[] for foo in range(self.height)]

		self.slits = [False] * (self.width - 1)
		self.endpoints = [0] * self.height

		self._row = 0

	@property
	def row(self):
		try:
			return self.rows[self._row]
		except IndexError:
			return []

	@property
	def endpoint(self):
		return self.endpoints[self._row]

	@endpoint.setter
	def endpoint(self, value):
		self.endpoints[self._row] = value

	def set_slit(self, i, value=True):
		if i < self.width:
			self.slits[i-1] = value

	def put_brick(self, length):
		self.row.append(length)
		self.endpoint += length
		if len(self.row) < self.n:
			self.set_slit(self.endpoint, True)

		else:
			self._row += 1

	def pop_brick(self):
		if not self.row:
			self._row -= 1

		length = self.row.pop()
		self.set_slit(self.endpoint, False)
		self.endpoint -= length
		

	def valid(self, length):
		next_end = self.endpoint + length
		if next_end < self.width:
			if self.slits[next_end-1]:
				return False

		if length in self.row:
			return False

		return True

	def _get_image(self):
		image = Image.new("RGB", (size*self.width, size*self.height), WHITE)
		draw = ImageDraw.Draw(image)
		for y, row in enumerate(self.rows):
			x = 0
			for length in row:
				draw.rectangle((x*size, y*size, (x+length)*size, (y+1)*size), outline=BLACK)
				x += length

		return image

	def save(self, filename):
		self._get_image().save(filename)

	def show(self):
		self._get_image().show()

	def hash(self, mirror=False):
		hash_row = lambda row: sum(digit * self.n**k for k, digit in enumerate(reversed(row) if mirror else row))
		hashed_rows = tuple(hash_row(row) for row in sorted(self.rows, key=hash_row))
		return hash(hashed_rows)


def solve(n):
	wall = Wall(n)
	depth = 0
	count = 0
	max_depth = wall.height * n
	already_found = []

	def push_brick(length=None):
		nonlocal wall, depth, count
		if length is not None:
			wall.put_brick(length)
			depth += 1

		if depth == max_depth:
			h1 = wall.hash()
			h2 = wall.hash(	mirror=True)
			if h1 not in already_found and h2 not in already_found:
				already_found.append(h1)
				already_found.append(h2)
				count += 1
				print("Found", count, end='\r')
				# wall.save("allSolutions/solution{}_{}.png".format(n, count))

		else:
			for i in range(1, n+1):
				if wall.valid(i):
					if push_brick(i):
						return True

		if length is not None:
			wall.pop_brick()
			depth -= 1

		return False

	if push_brick():
		return wall

	return None

solve(int(sys.argv[1]))
print()
