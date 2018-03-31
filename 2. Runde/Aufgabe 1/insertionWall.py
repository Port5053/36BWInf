
class Row:
	def __init__(self, bricks=[]):
		bricks = list(bricks)
		self.n = len(bricks)
		self.bricks = bricks
		self.slits = [sum(bricks[:i]) for i in range(self.n)]
		self.ins_index = None

	def expand(self):
		self.n += 1
		self.ins_index = self.n

		next_slit = self.slits[-1] + self.bricks[-1] if self.n > 1 else 0

		self.slits.append(next_slit)
		self.bricks.append(self.n)

		return next_slit

	def _shift_insertion(self):
		prev = self.bricks[self.ins_index-1]
		self.bricks[self.ins_index] = prev
		self.bricks[self.ins_index-1] = self.n

		self.slits[self.ins_index] += (self.n - prev)

		self.ins_index -= 1

		return self.slits[self.ins_index+1]

	def insertion_shifts(self):
		for i in range(self.n-1):
			yield self._shift_insertion()

	def __bool__(self):
		return bool(self.bricks)

	def __repr__(self):
		return "Row([{}])".format(", ".join(str(b) for b in self.bricks))

class Wall:
	def __init__(self):
		self.n = 0
		self.width = 0
		self.slits = []
		self.rows = [Row()]

	def increment(self):
		self.n += 1
		self.width += self.n
		add_row = (self.n % 2 == 0)

		self.slits.extend([False]*self.n)

	def shift_rows(self, add_row=False):
		depth = 0

		def shift(i):
			nonlocal depth
			depth += 1

			if depth == len(self.rows):  # TODO replace len with statement
				if add_row:
					next_row = self.construct_new_row()
					if next_row is None:
						return False

					self.rows.append(next_row)

				return True

			for slit in self.rows[i].insertion_shifts():

	def is_slit(self, x):
		if 0 < x < self.width:
			return self.slits[x]

		return False

	def construct_new_row(self):
		used = [False] * (self.n+1)
		row = []
		end = 0

		def add_brick(brick=0):
			nonlocal end
			used[brick] = True
			end += brick

			if end == self.width:
				return True

			for b in range(1, self.n+1):
				if not used[b] and not self.is_slit(end+b):
					if add_brick(b):
						row.append(b)
						return True

			used[brick] = False
			end -= brick

			return False

		if add_brick():
			return Row(reversed(row))

		return None

	def push_slits(self, row):
		for slit in row.slits:
			self.slits[slit] = True
