#pragma once

#ifndef _GA_H
#define _GA_H

#include <genotype.h>

typedef void (*eval_genotype_func)(Genotype *);
typedef void (*mutate_genotype_func)(Genotype *);
typedef void (*crossover_genotypes_func)(Genotype *, Genotype *, Genotype *);

typedef struct problem_t {
  int population_size;
  Genotype **population;
  eval_genotype_func eval_genotype;
  mutate_genotype_func mutate_genotype;
  crossover_genotypes_func crossover_genotypes;
} Problem;

Problem *init_problem(int,int,int,eval_genotype_func,mutate_genotype_func,crossover_genotypes_func);
void free_problem(Problem *);
void run_problem_up_to_generation(Problem *, int);
void print_genotype(Genotype *);

#endif