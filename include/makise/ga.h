#pragma once

#ifndef _GA_H
#define _GA_H

#include <genotype.h>
#include <stdio.h>
#include <mutation.h>
#include <crossover.h>
#include <logging.h>
#include <parameters.h>

typedef void (*eval_genotype_func)(Genotype *);

typedef struct problem_t {
  int population_size;
  Genotype **population;
  eval_genotype_func eval_genotype;
  int n_mutation_funcs;
  int n_crossover_funcs;
  mutate_genotype_func *mutate_genotype;
  crossover_genotypes_func *crossover_genotypes;
  log_step_func log_step;
  int tournament_size;
  double mutation_rate;
  FILE *output;
  int current_gen;
  Genotype **last_good_population;
  int partitions;
} Problem;

Problem *init_problem(Parameters *, eval_genotype_func);
void free_problem(Problem *);
void free_population(Genotype **, int);
void run_problem_until_convergence(Problem *);
void run_problem_up_to_generation(Problem *, int);

#endif
