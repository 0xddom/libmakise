#include <makise.h>

/*
 * This example evolves a piece of code that is
 * only composed by ones.
 */
void makise_eval_one_genotype(Genotype *g) {
  if (!g->evaluated) {
    g->evaluated = true;
    int i;
    
    for (i = 0; i < g->length; i++) {
      char byte = g->dna[i];
      if (byte > 0x20 && byte < 0x7f) g->fitness.hits++;
    }
    
    g->fitness.value = (double)g->fitness.hits / (g->length);
  }
}
