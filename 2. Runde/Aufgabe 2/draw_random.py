#!/usr/bin/python3
import sys
import random
n = int(sys.argv[1])
print(n)
for _ in range(n):
	for _ in range(n):
		print("{:.3f}".format(2*random.random()), end=' ')

	print()