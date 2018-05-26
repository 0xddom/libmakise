#pragma once

#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <mutation.h>
#include <stdio.h>
#include <logging.h>
#include <crossover.h>

typedef struct parameters_t {
  int population_size;
  int n_mutation_funcs;
  int n_crossover_funcs;
  mutate_genotype_func *mutation_algo;
  crossover_genotypes_func *crossover_algo;
  int seed;
  int dna_length;
  int generations;
  int tournament_size;
  double mutation_rate;
  FILE *output;
  log_step_func logger;
  char *prog_name;
  bool use_csv;
  bool do_restore;
  FILE *restore_file;
  bool default_seed;
  int partitions;
  int cromosomes;
  int n_mitocondrial;
} Parameters;

Parameters *parse_parameters(int,char **);

#endif
