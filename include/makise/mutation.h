#pragma once

#ifndef _MUTATION_H
#define _MUTATION_H

#define mutation_func(name) void name(Genotype *, double)

#include <genotype.h>

typedef void (*mutate_genotype_func)(Genotype *, double);

mutate_genotype_func get_mutation_func(char *);
void print_available_mutation_algorithms(void);

mutation_func(bit_flip);
mutation_func(exchange);
mutation_func(rewrite);

#endif
