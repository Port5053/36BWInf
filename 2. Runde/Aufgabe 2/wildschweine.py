#!/usr/bin/python3

# 36. BWInf Aufgabe 2
# Wehret den Wildschweinen!
# Kamal Abdellatif

def read_matrix(filename):
	"""Liest eine Matrix im Format der gegebenen Beispiele
	ein und gibt diese als 2D-Liste von floats zurueck"""
	matrix = []
	with open(filename, 'r') as fobj:
		fobj.readline()  # Ueberspringen der ersten Zeile
		for line in fobj.readlines():
			line = line[:-2]  # Entfernen von Space und newline
			if not line: break  # Leerzeile am Ende
			row = [float(num_string) for num_string in line.split(' ')]

			matrix.append(row)

	return matrix

class Meta(type):
	def __getitem__(cls, args):
		return cls.node_dict[args]

class Node(metaclass=Meta):
	node_dict = {}  # 2D Dict aller Instanzen
	def __init__(self, x, y):
		Node.node_dict[(x, y)] = self
		self.x, self.y = x, y
		self.edges = []

	def draw_edge(a, b, weight=0):
		a.edges.append((b, weight))
		b.edges.append((a, weight))

	def __repr__(self):
		return "Node({s.x:d}, {s.y:d})".format(s=self)

def generate_grid_graph(matrix):
	"""Generiert einen Graphen mit Grenzen als Kanten"""
	n = len(matrix)

	# Generieren der Knoten
	for y in range(1, n):
		for x in range(n+1):
			Node(x, y)

	# Horizontale Grenzen
	for y in range(n-1):
		for x in range(n):
			a, b = matrix[y][x], matrix[y+1][x]
			work = max(0, .5 * (1 - abs(a-b)))
			Node.draw_edge(Node[x, y+1], Node[x+1, y+1], work)

	# Vertikale Grenzen
	for y in range(1, n-1):
		for x in range(n-1):
			a, b = matrix[y][x], matrix[y][x+1]
			work = max(0, .5 * (1 - abs(a-b)))
			Node.draw_edge(Node[x+1, y], Node[x+1, y+1], work)

