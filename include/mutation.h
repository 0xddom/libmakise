#pragma once

#ifndef _MUTATION_H
#define _MUTATION_H

#include <genotype.h>

typedef void (*mutate_genotype_func)(Genotype *, double);

mutate_genotype_func get_mutation_func(char *);
void print_available_mutation_algorithms(void);

void bit_flip (Genotype *, double);

#endif
