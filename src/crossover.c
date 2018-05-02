#include <crossover.h>
#include <stdio.h>
#include <errors.h>
#include <stdlib.h>
#include <string.h>

void xor_crossover(Genotype *parent_a,
		   Genotype *parent_b,
		   Genotype *child) {
  int i;

  for (i = 0; i < parent_a->length; i++) {
    child->dna[i] = parent_a->dna[i] ^ parent_b->dna[i];
  }

}

void one_point(Genotype *parent_a, Genotype *parent_b, Genotype *child) {
  int i, p = rand() % parent_a->length;

  for (i = 0; i < parent_a->length; i++) {
    child->dna[i] = (i < p) ? parent_a->dna[i] : parent_b->dna[i];
  }
}

void two_point(Genotype *parent_a, Genotype *parent_b, Genotype *child) {
  int i;
  int p = rand() % parent_a->length;
  int q = rand() % parent_a->length;

  if (p > q) {
    int c = p;
    p = q;
    q = c;
  }

  for (i = 0; i < parent_a->length; i++) {
    child->dna[i] = (i < p || q < i) ?
      parent_a->dna[i] :
      parent_b->dna[i];
  }
}

// TODO: 50:50 crossover

struct crossover_table_name {
  const char *name;
  crossover_genotypes_func func;
  const char *desc;
};

static struct crossover_table_name table[] = {
  { "xor", xor_crossover, "Xors the dna of the two parents." },
  { "one-point", one_point, "Picks a point between the dna and splits the parents" },
  { "two-point", two_point, "Picks two points between the dna and splits the parents" },
  { NULL, NULL, NULL }
};

crossover_genotypes_func get_crossover_func(char *name) {
  int i;

  for (i = 0; table[i].name != NULL; i++) {
    if (strcmp (name, table[i].name) == 0) {
      return table[i].func;
    }
  }

  fprintf (stderr, "Crossover algorithm \"%s\" not recognized\n", name);
  exit (UNK_PARAM);
}

void print_available_crossover_algorithms() {
  int i;
  for (i = 0; table[i].name != NULL; i++) 
    printf ("\t%s: %s\n", table[i].name, table[i].desc);
}
