### If you do queries over and over, maybe you can speed up it with data structures:

- query if one item exists in a whole range? try `hash table`.
- query max/min in a whole range? try `heap`.
- query max/min/etc in a sub-range? try `segment tree`.
- query if two items are in the same set? try `union find`(see the next chapter).


## Pit-falls about segment trees:
- A segment tree can has at most 4 times of nodes than its coresponding ranges.
- Do not mix up the node index in the segment trees with the index of original ranges.
- When quering from it, use the results of previous sub queries, not the ones from children(these are result for those cover their whole range). see P3-TwoSum/2sum.cL96 (I introduced a new struct to store the result for each node in the segment tree because we need not just one value from the sub-queries).
