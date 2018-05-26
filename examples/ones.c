#include <makise.h>
#include <stdlib.h>

/*
 * This example evolves a piece of code that is
 * only composed by ones.
 */
void makise_eval_one_genotype(Genotype *g) {
  if (!g->evaluated) {
    g->evaluated = true;

    int s;

    gene *dna = get_usable_dna (g, &s);
    int bytesize = sizeof (char) * 8;
    int i;
    
    for (i = 0; i < s; i++) {
      char byte = dna[i];
      while (byte != 0) {
	if ((byte & 0x80) == 0x80) g->fitness.hits++;
	byte = byte << 1;
      }
    }

    free (dna);
    g->fitness.value = (double)g->fitness.hits / (s * bytesize);
  }
}
