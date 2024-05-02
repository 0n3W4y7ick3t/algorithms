### New stuff:
1. Huffman encoding uses a binary tree to compress chars, based on the frequency of a char,
if it's high, the encoding generated for it should be shorter to save more space.
(None of the two encodings have common prefix so no ambiguity when running through the encoded binary stream)
2. a hash that maps words to where they appear: this is called a *inverted index*, and it's useful in search engines.
3. read that MapReduce paper published by google, it's about distributed algorithms.
4. **Bloom filters and HyperLogLog** : *probabilistic* DSA and algo. Hash tables are great, gives you O(1) for query existence, but when the set becomes huge, It's maybe better to sacrifice some performance and confidence, for an answer that's probably correct.
5. SimHash is a locality-sensitive hash function, that is, if you change the strings a little bit, the new hash will also changed only a little bit, you can use the hash as some feature of the original string: you can say if two SimHashes are similar, then the original strings are also similar. But typically you want the hash to be totally different to prevent hash attacks.
6. Linear programming: just some high school problems that give some linear constrains(that limits what values can x and y be), and to find the max/min of another F = ax + by. But implement it in programming maybe tricky, checkout *Simplex Algorithms*.

### P, NP, NP-hard, NP-complete
(I dont see much meaning of defining these)
- A problem is in P if it is both quick to solve and quick to verify.
- A problem is in NP if it is quick to verify. It may or may not be quick to solve.
- A problem is NP-hard if any problem in NP can be reduced to that problem.
- If a problem is in both NP and NP-hard, it is NP-complete.
