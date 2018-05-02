#pragma once

#ifndef _GENOTYPE_H
#define _GENOTYPE_H

#include <stdbool.h>

typedef struct fitness_t {
  int hits;
  double value;
} Fitness;

typedef struct genotype_t {
  int length;
  char *dna;
  bool evaluated;
  Fitness fitness;
} Genotype;

Genotype *create_empty_genotype(int);
Genotype *create_random_genotype(int);
void print_genotype(Genotype *);

#endif
