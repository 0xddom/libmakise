#include <logging.h>

void log_step_in_csv(Genotype *best, int generation, FILE *f) {
  static bool headers_written = false;

  if (!headers_written) {
    fprintf (f, "generation,hits,fitness\n");
    headers_written = true;
  }

  fprintf (f, "%d,%d,%g\n", generation, best->fitness.hits, best->fitness.value);
}

void plain_text_log(Genotype *best, int generation, FILE *f) {
  fprintf (f, "Generation: %d\n\t", generation);
  print_genotype (best, f);
}
