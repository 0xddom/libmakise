#pragma once

#ifndef _CROSSOVER_H
#define _CROSSOVER_H

#include <genotype.h>

typedef void (*crossover_genotypes_func)(Genotype *, Genotype *, Genotype *);

crossover_genotypes_func get_crossover_func(char *);
void print_available_crossover_algorithms(void);

void xor_crossover(Genotype *, Genotype *, Genotype *);

#endif
