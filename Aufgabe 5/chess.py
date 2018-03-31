
dim = 8
headings = UP, DOWN, LEFT, RIGHT = 1, 2, 3, 4

class _MetaBoard(type):
	board = [[None for y in range(dim)] for x in range(dim)]

	@classmethod
	def __getitem__(cls, pos):
		x, y = pos
		return cls.board[x][y]

	@classmethod
	def __setitem__(cls, pos, arg):
		x, y = pos
		cls.board[x][y] = arg


class Node(metaclass=_MetaBoard):
	def __init__(self, x, y):
		self.x, self.y = x, y
		Node[x, y] = self

	@staticmethod
	def in_bounds(x, y):
		return 0 <= x < dim and 0 <= y < dim

	def headed_pos(self, heading):
		pass

	def neighbours(self):
		n_pos = (
			(self.x+1, self.y),
			(self.x, self.y+1),
			(self.x-1, self.y),
			(self.x, self.y-1),
		)
		for x, y in n_pos:
			if self.in_bounds(x, y):
				yield Node[x, y]

	def line(self, length, heading):
		x_range, y_range = {
			(length, length),
			(length, length),
			(length, length),
			(length, length),
		}

	def __repr__(self):
		return "Node({}, {})".format(self.x, self.y) 



