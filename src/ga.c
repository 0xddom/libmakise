#include <ga.h>
#include <stdlib.h>
#include <stdio.h>
#include <errors.h>

/**
 * Initializes a problem with a random population.
 */
Problem *init_problem(int population_size,
		      int dna_length,
		      int seed,
		      eval_genotype_func eval_genotype,
		      mutate_genotype_func mutate_genotype,
		      crossover_genotypes_func crossover_genotypes) {
  Problem *p = (Problem*)malloc (sizeof (Problem));
  int i;

  if (p == NULL) {
    fprintf (stderr, "Can't allocate space for the problem\n");
    exit (ALLOC_ERROR);
  }
  
  p->population_size = population_size;
  p->eval_genotype = eval_genotype;
  p->mutate_genotype = mutate_genotype;
  p->crossover_genotypes = crossover_genotypes;

  p->population = (Genotype**)malloc (sizeof (Genotype*) * population_size);

  if (p->population == NULL) {
    fprintf (stderr, "Can't allocate space for the population\n");
    free (p);
    exit(ALLOC_ERROR);
  }

  srand (seed);
  for (i = 0; i < p->population_size; i++) {
    p->population[i] = create_random_genotype (dna_length); 
  }

  return p;  
}

void run_problem_up_to_generation(Problem *p, int generations) {
  int i;

  for (i = 0; i < p->population_size; i++) {
    p->eval_genotype (p->population[i]);
  }
}

void free_genotype(Genotype *g) {
  free (g->dna);
  free (g);
}

void free_problem(Problem *p) {
  int i;

  for (i = 0; i < p->population_size; i++) {
    free_genotype (p->population[i]);
  }
  
  free (p);
}
