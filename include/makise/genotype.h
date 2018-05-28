#pragma once

#ifndef _GENOTYPE_H
#define _GENOTYPE_H

#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#define FEMALE true
#define MALE false

#define dna_size(g) (1 + g->dna_length * g->n_cromosomes)

typedef bool gender;
typedef uint8_t gene;

typedef struct fitness_t {
  int hits;
  double value;
} Fitness;

typedef struct genotype_t {
  int length;
  int dna_length;
  gene *dna;
  int n_cromosomes;
  int n_mitocondrial;
  bool evaluated;
  int head;
  Fitness fitness;
} Genotype;

Genotype *create_empty_genotype(int,int,int,int);
Genotype *create_random_genotype(int,int,int,int);
void print_genotype(Genotype *, FILE *f);
gender get_gender(Genotype *);
void set_gender(Genotype *, gender);
gene *get_usable_dna(Genotype *, int *);
void copy_mitocondrial_dna(Genotype *, Genotype *);

#endif
