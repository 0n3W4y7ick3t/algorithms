about input samples:
-----------------
number of cases

number of nodes
destination node
time limit
number of edges
from to weight
...more edges


...more cases
-----------------

we treat the destination node as the start, so we can run one dijkstra from that node
and get all cost to all nodes possible. so the edge "from to weight" should be read as "to from weight".
