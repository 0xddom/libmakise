#include <makise.h>

/*
 * This example evolves a piece of code that is
 * only composed by ones.
 */
void makise_eval_one_genotype(Genotype *g) {
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
