# Makise

Makise is a library for running genetic algorithms. When time permits I want to expand and clean-up the code and the public API.

The internal representation of the genotypes is the very common binary string.

Steins;Gate 0 was airing when I made this, hence the name.

## GEP

Parameters that seems to work the best for the 3-regression problem:

* Crossover function: two-point or one-point randomly choosen
* Mutation function: bit-flip
* Islands: <10
* Number of mitocondrial genes: ~20
* DNA length: 31
* Number of ADFs: 0, they are not needed.
* Population: In the order of 1k

The rest of the parameters are the default ones.
