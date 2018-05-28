#pragma once

#ifndef _GEP_FENOTYPE_H
#define _GEP_FENOTYPE_H

#include <makise/genotype.h>
#include <gep/tree.h>

typedef struct {
  Genotype *genotype;
  Tree **adfs;
  Tree *tree;
} Fenotype;

Fenotype *genotype_to_fenotype(Genotype*);
void free_fenotype(Fenotype *);

#endif
