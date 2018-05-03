#include <mutation.h>
#include <stdio.h>
#include <errors.h>
#include <string.h>
#include <stdlib.h>

#define RAND_MAX_RANGE 100

void bit_flip(Genotype *g, double mutation_rate) {
  double r = 1.0 / ((rand() % RAND_MAX_RANGE) + 1);
  if (r < mutation_rate) {
    int i = rand() % g->length;
    int j = rand() % 8;

    g->dna[i] ^= 1UL << j;
  }
}

struct mutation_table_name {
  const char *name;
  mutate_genotype_func func;
  const char *desc;
};

static struct mutation_table_name table[] = {
  { "bitflip", bit_flip, "Flips a random number of bits of the dna." },
  { NULL, NULL, NULL }
};

mutate_genotype_func get_mutation_func(char *name) {
  int i;

  for (i = 0; table[i].name != NULL; i++) {
    if (strcmp (name, table[i].name) == 0) {
      return table[i].func;
    }
  }

  fprintf (stderr, "Mutation algorithm \"%s\" not recognized\n", name);
  exit (UNK_PARAM);
}

void print_available_mutation_algorithms() {
  int i;
  for (i = 0; table[i].name != NULL; i++) 
    printf ("\t%s: %s\n", table[i].name, table[i].desc);
}
