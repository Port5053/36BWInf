from shortest_path import *

import matplotlib.pyplot as plt
import sys; i = sys.argv[1]
matrix = read_matrix("eingaben/wildschwein%s.txt" % i)
n = len(matrix)
s, t = generate_grid_graph(matrix)
path = dijkstra(s, t)

ax = plt.gca()
ax.invert_yaxis()
plt.axis("off")
plt.tight_layout()
plt.imshow(matrix, cmap="gray", origin="lower", extent=(0, n, 0, n))
plt.plot(*zip(*path), "red", linewidth=5)
plt.savefig("path%s.png" % i, bbox_inches="tight", pad_inches=0) 