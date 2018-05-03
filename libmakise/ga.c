#include <ga.h>
#include <stdlib.h>
#include <stdio.h>
#include <errors.h>
#include <assert.h>
#include <stdbool.h>

void free_genotype(Genotype *g) {
  free (g->dna);
  free (g);
}

void free_population(Genotype **pop, int pop_size) {
  int i;

  for (i = 0; i < pop_size; i++) {
    free_genotype (pop[i]);
  }
}

/**
 * Initializes a problem with a random population.
 */
Problem *init_problem(int population_size,
		      int dna_length,
		      int seed,
		      int tournament_size,
		      double mutation_rate,
		      eval_genotype_func eval_genotype,
		      mutate_genotype_func mutate_genotype,
		      crossover_genotypes_func crossover_genotypes,
		      log_step_func log_step,
		      FILE *output) {
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
  p->tournament_size = tournament_size;
  p->mutation_rate = mutation_rate;
  p->log_step = log_step;
  p->output = output;

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

bool genotype_in_population(Genotype *g, Genotype **lst, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (g == lst[i]) return true;
  }
  return false;
}

Genotype *random_unique_genotype_from_population(int pop_size, Genotype **population, Genotype **already_selected, int selected_size) {
  Genotype *candidate;

  do {
    candidate = population[rand() % pop_size];
  } while (genotype_in_population (candidate, already_selected, selected_size));

  return candidate;
}

Genotype *tournament_selection(Problem *p) {
  Genotype *choosen_one = NULL;
  int i;
  Genotype **already_selected;

  already_selected = (Genotype**)calloc (sizeof (Genotype*), p->tournament_size);
  
  for (i = 0; i < p->tournament_size; i++) {
    Genotype *candidate;
    candidate = random_unique_genotype_from_population (p->population_size, p->population, already_selected, i);
    if (!candidate->evaluated) {
      p->eval_genotype(candidate);
      candidate->evaluated = true;
    }
    if (choosen_one == NULL || candidate->fitness.value > choosen_one->fitness.value) {
      choosen_one = candidate;
    }
  }

  free (already_selected);
  assert (choosen_one != NULL);
  
  return choosen_one;
}

void replace_generations(Problem *p, Genotype **new_population, Genotype **old_population) {
  p->population = new_population;
  free_population (old_population, p->population_size);
}

Genotype **run_generation_step(Problem *p, int generation) {
  int i;
  Genotype **new_population;

  new_population = (Genotype**)malloc (sizeof (Genotype *) * p->population_size);

  if (new_population == NULL) {
    fprintf (stderr, "Can't allocate space for the new population in generation %d\n", generation);
    exit (ALLOC_ERROR);
  }

  for (i = 0; i < p->population_size; i++) {
    Genotype *parents[2];
    Genotype *child;

    parents[0] = tournament_selection (p);
    do {
      parents[1] = tournament_selection (p);
    } while (parents[0] == parents[1]);
    child = create_empty_genotype (parents[0]->length);

    p->crossover_genotypes (parents[0], parents[1], child);
    p->mutate_genotype (child, p->mutation_rate);

    new_population[i] = child;
  }

  return new_population;
}

bool problem_has_converged(Problem *p) {
  int i;

  for (i = 0; i < p->population_size; i++) {
    if (p->population[i]->fitness.value >= 1) return true;
  }
  return false;
}

Genotype *get_best(Problem *p) {
  Genotype *best;
  int i;

  best = NULL;
  for (i = 0; i < p->population_size; i++) {
    if (!p->population[i]->evaluated) p->eval_genotype (p->population[i]);
    if (best == NULL || best->fitness.value < p->population[i]->fitness.value)
      best = p->population[i];
  }

  assert (best != NULL);
  
  return best;
}

void run_problem_up_to_generation(Problem *p, int generations) {
  int i;
  Genotype **new_population;
  Genotype *best;
  
  for (i = 0; i < generations; i++) {
    new_population = run_generation_step (p, i);
    best = get_best (p);
    p->log_step (best, i, p->output);
    if (problem_has_converged (p)) break;
    replace_generations (p, new_population, p->population);
  }

  best = get_best (p);
  fprintf (stderr, "The problem has finished at %d generation(s)\n", i+1);
  fprintf (stderr, "The best fitness is:\n\t");
  print_genotype (best, stderr);
}



void free_problem(Problem *p) {
  free_population (p->population, p->population_size);
  if (!(p->output == stdout || p->output == stderr))
    fclose (p->output);
  free (p);
}
