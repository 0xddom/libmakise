#include <makise.h>
#include <fenotype.h>
#include <stdlib.h>

void makise_eval_one_genotype(Genotype *g) {
  Fenotype *f;

  f = genotype_to_fenotype (g);

  free_fenotype (f);

  exit (0);
}
