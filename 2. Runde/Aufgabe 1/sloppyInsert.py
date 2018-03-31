def check(wall):
	n = len(wall[0])
	width = n*(n+1)//2
	slits = [False] * width

	for row in wall:
		x = 0
		for brick in row:
			x += brick
			if x < width:
				if slits[x]:
					return False

				slits[x] = True

	return True


def construct_new_row(wall):
	n = len(wall[0])
	width = n*(n+1)//2
	slits = [False] * width
	for row in wall:
		x = 0
		for brick in row:
			x += brick
			if x < width:
				slits[x] = True

	used = [False] * (n+1)
	row = []
	end = 0

	is_slit = lambda x: slits[x] if x < width else False

	def add_brick(brick=0):
		nonlocal end
		used[brick] = True
		end += brick

		if end == width:
			return True

		for b in range(1, n+1):
			if not used[b] and not is_slit(end+b):
				if add_brick(b):
					row.append(b)
					return True

		used[brick] = False
		end -= brick

		return False

	if add_brick():
		return reversed(row)

	return None

wall = [
	[1, 2, 3, 4],
	[2, 3, 4, 1],
]


print(*construct_new_row(wall))
