#include <stdio.h>
#include <ga.h>
#include <mutation.h>
#include <crossover.h>
#include <parameters.h>
#include <stdbool.h>
#include <signals.h>
#include <serialization.h>
#include <stdlib.h>
#include <errors.h>

void makise_eval_one_genotype(Genotype *g);

int main(int argc, char **argv) {
  Parameters *params;
  Problem *p;

  params = parse_parameters (argc, argv);
  
  p = init_problem (params, makise_eval_one_genotype);

  if (params->do_restore) {
    fseek (params->restore_file, 0, SEEK_END);
    int restored_state_length = ftell (params->restore_file);
    fseek (params->restore_file, 0, SEEK_SET);

    void *buf = malloc (restored_state_length);
    fread (buf, restored_state_length, 1, params->restore_file);
    
    if (deserialize_program_state (&params, &p, buf, restored_state_length) < 0) {
      fprintf (stderr, "Error while loading restore file.\n");
      exit (ALLOC_ERROR);
    }
    
    fclose (params->restore_file);
  }
  
  setup_signals (params, p);
  
  if (params->generations < 0) run_problem_until_convergence (p);
  else run_problem_up_to_generation (p, params->generations);
  free_problem (p);

  return 0;
}
