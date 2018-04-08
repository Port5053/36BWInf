#!/usr/bin/python3

from heapq import *

# 36. BWInf Aufgabe 2
# Wehret den Wildschweinen!
# Kamal Abdellatif

__all__ = ["read_matrix", "generate_grid_graph", "dijkstra"]

def read_matrix(filename):
    """Liest eine Matrix im Format der gegebenen Beispiele
    ein und gibt diese als 2D-Liste von floats zurueck"""
    matrix = []
    with open(filename, 'r') as fobj:
        fobj.readline()  # Ueberspringen der ersten Zeile
        for line in fobj.readlines():
            line = line[:-2]  # Entfernen von Space und newline
            if not line:
                break  # Leerzeile am Ende
            row = [float(num_string) for num_string in line.split(' ')]
            matrix.append(row)

    return matrix


class Meta(type):
    def __getitem__(cls, args):
        return cls.node_dict[args]


class Node(metaclass=Meta):
    node_dict = {}  # 2D Dict aller Instanzen
    vertices = []
    def __init__(self, x=None, y=None, virtual=False):
        Node.vertices.append(self)
        if not virtual:
            Node.node_dict[(x, y)] = self
            self.pos = x, y

        self.virtual = virtual
        self.edges = []

    def draw_edge(a, b, weight=0):
        a.edges.append((b, weight))
        b.edges.append((a, weight))

    def __lt__(a, b):
        """Dummy for heapify"""
        return True

    def __repr__(self):
        if self.virtual:
            return "Node(virtual=True)"

        return "Node({s.x:d}, {s.y:d})".format(s=self)


def generate_grid_graph(matrix):
    """Generiert einen Graphen mit Grenzen als Kanten.
       Gibt die Virtuellen Knoten s, t zurueck"""
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
            Node[x, y+1].draw_edge(Node[x+1, y+1], work)

    # Vertikale Grenzen
    for y in range(1, n-1):
        for x in range(n-1):
            a, b = matrix[y][x], matrix[y][x+1]
            work = max(0, .5 * (1 - abs(a-b)))
            Node[x+1, y].draw_edge(Node[x+1, y+1], work)

    s = Node(virtual=True)
    t = Node(virtual=True)
    for y in range(1, n):
        s.draw_edge(Node[0, y])
        t.draw_edge(Node[n, y])

    return s, t

def dijkstra(s, t):
    for vertex in Node.vertices:
        vertex.seen = False
        vertex.parent = None
        vertex.dist = float("inf")

    queue = [(0, s)]
    s.dist = 0

    vertex = None
    while queue:
        dist, vertex = heappop(queue)
        vertex.seen = True
        for child, weight in vertex.edges:
            if child.dist > dist + weight:
                child.dist = dist + weight
                child.parent = vertex

                if not child.seen:
                    heappush(queue, (child.dist, child))

    node = t
    path = []
    while node is not s:
        node = node.parent
        if not node.virtual:
            path.append(node.pos)

    path.reverse()
    return path