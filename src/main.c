#include <stdio.h>
#include <time.h>
#include <ga.h>
#include <mutation.h>
#include <crossover.h>
#include <parameters.h>

void test_function(Genotype *g) {
  if (!g->evaluated) {
    g->evaluated = true;
    int bytesize = sizeof (char) * 8;
    int i;
    
    for (i = 0; i < g->length; i++) {
      char byte = g->dna[i];
      while (byte != 0) {
	if ((byte & 0x80) == 0x80) g->fitness.hits++;
	byte = byte << 1;
      }
    }
    
    g->fitness.value = (double)g->fitness.hits / (g->length * bytesize);
  }
}


int main(int argc, char **argv) {
  Parameters *params;
  Problem *p;

  params = parse_parameters (argc, argv);
  
  p = init_problem (params->population_size,
		    params->dna_length,
		    params->seed,
		    params->tournament_size,
		    params->mutation_rate,
		    test_function,
		    params->mutation_algo,
		    params->crossover_algo);

  run_problem_up_to_generation (p, params->generations);
  free_problem (p);

  return 0;
}
