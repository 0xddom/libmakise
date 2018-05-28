#include <ga.h>
#include <stdlib.h>
#include <stdio.h>
#include <errors.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

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

static void update_islands(Problem *p) {
  int i, j;

  for (i = 0, j = 0; i < p->population_size; i += p->partition_size, j++) {
    p->islands[j] = &(p->population[i]);
  }
}

/**
 * Initializes a problem with a random population.
 */
Problem *init_problem(Parameters *params, eval_genotype_func eval_genotype) {
  Problem *p = (Problem*)malloc (sizeof (Problem));
  int i;

  if (p == NULL) {
    fprintf (stderr, "Can't allocate space for the problem\n");
    exit (ALLOC_ERROR);
  }

  assert (params->population_size > params->tournament_size);
  assert ((params->population_size % params->partitions) == 0);
  assert (((params->dna_length - 1) % params->arity) == 0);
  params->head = (params->dna_length - 1) / params->arity;

  p->population_size = params->population_size;
  p->eval_genotype = eval_genotype;
  p->mutate_genotype = params->mutation_algo;
  p->n_mutation_funcs = params->n_mutation_funcs;
  p->crossover_genotypes = params->crossover_algo;
  p->n_crossover_funcs = params->n_crossover_funcs;
  p->tournament_size = params->tournament_size;
  p->mutation_rate = params->mutation_rate;
  p->log_step = params->logger;
  p->output = params->output;
  p->current_gen = 0;
  p->partitions = params->partitions;
  p->partition_size = params->population_size / params->partitions;
  p->migration_time = params->migration;

  assert (p->partition_size > params->tournament_size);

  p->population = (Genotype**)malloc (sizeof (Genotype*) * params->population_size);
  p->last_good_population = p->population;

  if (p->population == NULL) {
    fprintf (stderr, "Can't allocate space for the population\n");
    free (p);
    exit (ALLOC_ERROR);
  }

  p->islands = (Genotype***)malloc (sizeof (Genotype**) * params->partitions);

  if (p->islands == NULL) {
    fprintf (stderr, "Can't allocate space for the island pointers\n");
    exit (ALLOC_ERROR);
  }

  update_islands (p);

  srand (params->seed);
  for (i = 0; i < p->population_size; i++) {
    p->population[i] = create_random_genotype (params->dna_length,
					       params->cromosomes,
					       params->n_mitocondrial,
					       params->head); 
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

Genotype *random_unique_genotype_from_population(int pop_size,
						 Genotype **population,
						 Genotype **already_selected,
						 int selected_size) {
  Genotype *candidate;

  do {
    candidate = population[rand () % pop_size];
  } while (genotype_in_population (candidate, already_selected, selected_size));

  return candidate;
}

Genotype *tournament_selection(Problem *p, Genotype **population, int population_size, gender gdr) {
  Genotype *choosen_one = NULL;
  int i, retries;
  Genotype **already_selected;

  retries = 0;

  already_selected = (Genotype**)calloc (sizeof (Genotype*), p->tournament_size);
  
  for (i = 0; i < p->tournament_size; i++) {
    Genotype *candidate;
    candidate = random_unique_genotype_from_population (population_size,
							population,
							already_selected,
							i);

    if (gdr != get_gender (candidate)) {
      retries++;
      // If too much retries without finding a valid candidate of the required gender
      if (retries >= p->tournament_size) {
	set_gender (candidate, gdr);
	retries = 0;
      } else {
	i--;
	continue;
      }
    }
    
    if (!candidate->evaluated) {
      p->eval_genotype (candidate);
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
  Genotype **clean = p->last_good_population;
  
  p->population = new_population;
  p->last_good_population = old_population;
  if (clean != p->last_good_population)
    free_population (clean, p->population_size);
  update_islands (p);
}

#define MOTHER 1

static void crossover(Problem *p, Genotype **parents, Genotype *child) {
  p->crossover_genotypes[(p->n_crossover_funcs > 1) ?
			 rand() % p->n_crossover_funcs :
			 0] (parents[0], parents[1], child);
  copy_mitocondrial_dna (parents[MOTHER], child);
}

static void mutate(Problem *p, Genotype *child) {
  p->mutate_genotype[(p->n_mutation_funcs > 1) ?
		     rand () % p->n_mutation_funcs :
		     0] (child, p->mutation_rate);
}

Genotype **run_generation_step(Problem *p, int generation) {
  int i, j;
  Genotype **new_population;
  Genotype *parents[2];
  Genotype *child;

  new_population = (Genotype**)malloc (sizeof (Genotype *) * p->population_size);

  if (new_population == NULL) {
    fprintf (stderr, "Can't allocate space for the new population in generation %d\n", generation);
    exit (ALLOC_ERROR);
  }

  for (j = 0; j < p->partitions; j++) {
    Genotype **population = p->islands[j];
    for (i = 0; i < p->partition_size; i++) {
      parents[0] = tournament_selection (p, population, p->partition_size, MALE);
      parents[1] = tournament_selection (p, population, p->partition_size, FEMALE);
      child = create_empty_genotype (parents[0]->dna_length,
				     parents[0]->n_cromosomes,
				     parents[0]->n_mitocondrial,
				     parents[0]->head);
      
      crossover (p, parents, child);
      mutate (p, child);
      
      new_population[i + j * p->partition_size] = child;
    }
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

static void end_step(Problem *p) {
  Genotype *best;

  best = get_best (p);
  fprintf (stderr, "The problem has finished in %d generation(s)\n", p->current_gen);
  fprintf (stderr, "The best fitness is:\n\t");
  print_genotype (best, stderr);
}

static Problem *problem;

static int compare_two_genotypes (Genotype **p_g1, Genotype **p_g2) {
  Genotype *g1 = *p_g1;
  Genotype *g2 = *p_g2;
  
  if (!g1->evaluated) {
    problem->eval_genotype (g1);
    g1->evaluated = true;
  }
  if (!g2->evaluated) {
    problem->eval_genotype (g2);
    g2->evaluated = true;
  }

  if (g1->fitness.value < g2->fitness.value) return  1;
  if (g1->fitness.value > g2->fitness.value) return -1;
  return 0;
}

#define max(a,b) ((a) > (b)) ? (a) : (b)

static int get_n_migrants(Problem *p) {
  return max(1, (int)floor (p->partition_size * 0.2));
}

static void migrate_islands(Problem *p) {
  int i, j;
  int migrants;
  int new_island;

  if (p->partitions == 1) return; // No need to migrate if there are no other islands
  
  // NOT THREAD-SAFE!!!
  problem = p;

  for (i = 0; i < p->partitions; i++) 
    qsort (p->islands[i],
	   p->partition_size,
	   sizeof (Genotype*),
	   (__compar_fn_t)compare_two_genotypes);

  migrants = get_n_migrants (p);
  for (i = 0; i < p->partitions; i++) {
    for (j = 0; j < migrants; j++) {
      do new_island = rand () % p->partitions; while (new_island == i);
      Genotype *tmp = p->islands[i][j];
      p->islands[i][j] = p->islands[new_island][j];
      p->islands[i][j] = tmp;
    }
  }

  problem = NULL;
}

static bool evo_step(Problem *p) {
  Genotype **new_population;
  Genotype *best;
  new_population = run_generation_step (p, p->current_gen);

  best = get_best (p);
  
  p->log_step (best, p->current_gen, p->output);

  if (problem_has_converged (p)) { p->current_gen++; return true; }

  replace_generations (p, new_population, p->population);
  if ((p->current_gen % p->migration_time) == 0) migrate_islands (p);

  return false;
}

void run_problem_up_to_generation(Problem *p, int generations) {
  for (; p->current_gen < generations; p->current_gen++) {
    if (evo_step (p)) break;
  }

  end_step (p);
}

void run_problem_until_convergence(Problem *p) {
  for (;; p->current_gen++) {
    if (evo_step (p)) break;
  }

  end_step (p);
}



void free_problem(Problem *p) {
  if (p->last_good_population != p->population)
    free_population (p->last_good_population, p->population_size);
  free_population (p->population, p->population_size);
  if (!(p->output == stdout || p->output == stderr))
    fclose (p->output);
  free (p->mutate_genotype);
  free (p->crossover_genotypes);
  free (p);
}
