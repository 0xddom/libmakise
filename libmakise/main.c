#include <stdio.h>
#include <ga.h>
#include <mutation.h>
#include <crossover.h>
#include <parameters.h>
#include <stdbool.h>

void makise_eval_one_genotype(Genotype *g);

int main(int argc, char **argv) {
  Parameters *params;
  Problem *p;

  params = parse_parameters (argc, argv);
  
  p = init_problem (params->population_size,
		    params->dna_length,
		    params->seed,
		    params->tournament_size,
		    params->mutation_rate,
		    makise_eval_one_genotype,
		    params->mutation_algo,
		    params->crossover_algo,
		    params->logger,
		    params->output);

  run_problem_up_to_generation (p, params->generations);
  free_problem (p);

  return 0;
}
