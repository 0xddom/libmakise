#pragma once

#ifndef _CROSSOVER_H
#define _CROSSOVER_H

#define crossover_func(name) void name(Genotype *, Genotype *, Genotype *)

#include <genotype.h>

typedef void (*crossover_genotypes_func)(Genotype *, Genotype *, Genotype *);

crossover_genotypes_func get_crossover_func(char *);
void print_available_crossover_algorithms(void);

crossover_func(xor_crossover);
crossover_func(one_point);
crossover_func(two_point);
crossover_func(uniform);
crossover_func(uniform_4);

#endif
