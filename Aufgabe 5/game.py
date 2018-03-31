class Board:
	def __init__(self, dim=8):
		self.dim = dim

	def in_bounds(self, x, y):
		return 0 <= x < self.dim and 0 <= y < self.dim

	def neighbours(self, x, y):
		n_pos = (
			(self.x+1, self.y),
			(self.x, self.y+1),
			(self.x-1, self.y),
			(self.x, self.y-1),
		)
		for pos in n_pos:
			if self.in_bounds(pos):
				yield pos

	def outgoing_lines(self, x, y):
		# TODO correct order
		for i in range(self.dim):
				yield i, y

		for i in range(self.dim):
				yield x, i


