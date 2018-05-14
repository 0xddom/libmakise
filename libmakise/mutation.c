#include <mutation.h>
#include <stdio.h>
#include <errors.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define RAND_MAX_RANGE 100

void bit_flip(Genotype *g, double mutation_rate) {
  int i,j,k,c;
  double r = 1.0 / ((rand () % RAND_MAX_RANGE) + 1);
  if (r < mutation_rate) {
    k = rand () % g->length;
    for (c = 0; c < k; c++) {
      i = rand () % g->length;
      j = rand () % 8;
      
      g->dna[i] ^= 1UL << j;
    }
  }
}

void exchange(Genotype *g, double mutation_rate) {
  double r = 1.0 / ((rand () % RAND_MAX_RANGE) + 1);
  if (r < mutation_rate) {
    int i = rand () % g->length;
    int j = rand () % g->length;
    uint8_t c = g->dna[i];
    g->dna[i] = g->dna[j];
    g->dna[j] = c;
  }
}

void rewrite(Genotype *g, double mutation_rate) {
  int p, j;
  assert ((g->length % 4) == 0);

  double r = 1.0 / ((rand () % RAND_MAX_RANGE) + 1);
  if (r < mutation_rate) {
    p = rand () % (g->length / 4);
    for (j = 0; j < 4; j++) {
      g->dna[p * 4 + j] = (uint8_t)rand ();
    }
  }
}

struct mutation_table_name {
  const char *name;
  mutate_genotype_func func;
  const char *desc;
};

static struct mutation_table_name table[] = {
  { "bitflip", bit_flip, "Flips a random number of bits of the dna." },
  { "exchange", exchange, "Exchanges two genes in the dna." },
  { "rewrite", rewrite, "Rewrites random 4 bytes of the dna." },
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

const char *get_mutation_func_name(mutate_genotype_func f) {
  int i;

  for (i = 0; table[i].func != NULL; i++) {
    if (f == table[i].func) {
      return table[i].name;
    }
  }

  fprintf (stderr, "Mutation algorithm function %p not recognized\n", f);
  exit (UNK_PARAM);
}
