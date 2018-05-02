#pragma once

#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <mutation.h>
#include <crossover.h>

typedef struct parameters_t {
  int population_size;
  mutate_genotype_func mutation_algo;
  crossover_genotypes_func crossover_algo;
  int seed;
  int dna_length;
  int generations;
  int tournament_size;
  double mutation_rate;
} Parameters;

Parameters *parse_parameters(int,char **);

#endif
