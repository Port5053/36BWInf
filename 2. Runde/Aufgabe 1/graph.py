from itertools import *
import matplotlib.pyplot as plt
import numpy as np

n = 20
width = n*(n+1)//2

bricks = range(1, n+1)

def compatible(p1, p2):
	s1, s2 = 0, 0
	p1 = iter(p1)
	p2 = iter(p2)
	for _ in range(2*(n-1)):
		if s1 < s2:
			s1 += next(p1)
		else:
			s2 += next(p2)
		if s1 == s2:
			return False
	return True


def plot_matrix():
	perms = list( permutations(bricks) )
	plot = np.zeros((len(perms), len(perms)), dtype=np.bool)
	for i in range(len(perms)):
		for j in range(len(perms)):
			plot[i, j] =  compatible(perms[i], perms[j])

	plt.imshow(plot, cmap="Greys_r", origin="lower")

def count_occurrences():
	occ = [0] * (width+1)
	for comb in chain(*(combinations(bricks, i) for i in bricks)):
		occ[sum(comb)-1] += 1

	return occ

def count_occurrences_philip():
	occ = [0] * (width+1)
	for comb in combinations_with_replacement(bricks, 10):
		occ[sum(comb)-1] += 1

	return occ


plt.plot(range(width+1), count_occurrences_philip())
plt.show()
