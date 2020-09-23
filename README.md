This is an implementation of the community detection algorithm described in **Fast unfolding of communities in large networks** (arXiv:0803.0476).

```
$ ./community_detection < test.in
number of nodes: 16
number of edges: 28
edge 1: 0 -> 2 (1)
edge 2: 0 -> 3 (1)
edge 3: 0 -> 4 (1)
edge 4: 0 -> 5 (1)
edge 5: 1 -> 2 (1)
edge 6: 1 -> 4 (1)
edge 7: 1 -> 7 (1)
edge 8: 2 -> 4 (1)
edge 9: 2 -> 5 (1)
edge 10: 2 -> 6 (1)
edge 11: 3 -> 7 (1)
edge 12: 4 -> 10 (1)
edge 13: 5 -> 7 (1)
edge 14: 5 -> 11 (1)
edge 15: 6 -> 7 (1)
edge 16: 6 -> 11 (1)
edge 17: 8 -> 9 (1)
edge 18: 8 -> 10 (1)
edge 19: 8 -> 11 (1)
edge 20: 8 -> 14 (1)
edge 21: 8 -> 15 (1)
edge 22: 9 -> 12 (1)
edge 23: 9 -> 14 (1)
edge 24: 10 -> 11 (1)
edge 25: 10 -> 12 (1)
edge 26: 10 -> 13 (1)
edge 27: 10 -> 14 (1)
edge 28: 11 -> 13 (1)

Result:
4 0 2 5 1 
3 6 7 
12 9 14 8 10 15 
13 11 
```
