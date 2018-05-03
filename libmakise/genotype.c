#include <genotype.h>
#include <stdlib.h>
#include <errors.h>

Genotype *create_empty_genotype(int dna_length) {
  Genotype *g = (Genotype*)malloc (sizeof (Genotype));

  if (g == NULL) {
    fprintf (stderr, "Can't allocate space for a random genotype\n");
    exit (ALLOC_ERROR);
  }
  
  g->length = dna_length;
  g->evaluated = false;
  g->fitness.hits = 0;
  g->fitness.value = 0.0;

  g->dna = (char*)malloc (sizeof (char) * dna_length);

  if (g->dna == NULL) {
    fprintf (stderr, "Can't allocate space for a dna segment\n");
    free (g);
    exit (ALLOC_ERROR);
  }

  return g;
}

Genotype *create_random_genotype(int dna_length) {
  int i;
  Genotype *g = create_empty_genotype (dna_length);

  for (i = 0; i < dna_length; i++) {
    g->dna[i] = (char)rand ();
  }
  
  return g;
}

void print_genotype(Genotype *g, FILE *f) {
  int i;

  fprintf (f, "Genotype (length=%d, dna=\"", g->length);
  for (i = 0; i < g->length; i++) {
    fprintf (f, "\\x%02hhx", g->dna[i]);
  }
  fprintf (f, "\", fitness={ hits=%d, value=%g })\n", g->fitness.hits, g->fitness.value);
  
}
