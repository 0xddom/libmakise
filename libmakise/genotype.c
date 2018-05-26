#include <genotype.h>
#include <stdlib.h>
#include <errors.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define NO_GENDER_MASK 0xfe
#define ORDER_MASK 2

/*
 * N: Number of cromosomes
 * D: Number of genes in a cromosome
 * M: Number of genes in a cromosome that are mitocondrial
 *
 *
 * DNA structure:
 * 
 *  - Metadata (1B)
 *  - Cromosomes: (N)
 *    - Genes ((D-M))
 *    - Mitocondrial genes (M)
 *
 * Total size: 1 + N * D Bytes
 * Min size: 1 + N
 */

Genotype *create_empty_genotype(int dna_length, int n_cromosomes, int n_mitocondrial) {
  Genotype *g = (Genotype*)malloc (sizeof (Genotype));

  assert (dna_length >= n_mitocondrial);
  if (g == NULL) {
    fprintf (stderr, "Can't allocate space for a random genotype\n");
    exit (ALLOC_ERROR);
  }

  g->dna_length = dna_length;
  g->n_cromosomes = n_cromosomes;
  g->n_mitocondrial = n_mitocondrial;
  g->length = dna_size (g);
  g->evaluated = false;
  g->fitness.hits = 0;
  g->fitness.value = 0.0;

  g->dna = (gene*)calloc (sizeof (gene), dna_size (g));

  if (g->dna == NULL) {
    fprintf (stderr, "Can't allocate space for the dna\n");
    free (g);
    exit (ALLOC_ERROR);
  }

  return g;
}

Genotype *create_random_genotype(int dna_length, int n_cromosomes, int n_mitocondrial) {
  int i;
  Genotype *g = create_empty_genotype (dna_length, n_cromosomes, n_mitocondrial);

  for (i = 0; i < g->length; i++) {
    g->dna[i] = (gene)rand ();
  }
  
  return g;
}

void copy_mitocondrial_dna(Genotype *mother, Genotype *child) {
  int i, j;

  for (i = 1, j = 0; i < dna_size (mother); i++, j++) {
    if (j >= (mother->dna_length - mother->n_mitocondrial)) {
      child->dna[i] = mother->dna[i];
    }
    if (j >= mother->dna_length) j = 0;
  }
}

gene *get_usable_dna(Genotype *g, int *size) {
  int i;
  gene *usable_dna;

  *size = g->dna_length * g->n_cromosomes;

  usable_dna = (gene*)malloc (*size);

  for (i = 0; i < *size; i++) {
    usable_dna[i] = g->dna[i+1];
  }

  return usable_dna;
}

#define use_colors(f) isatty(fileno(f))
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CLEAR "\x1b[0m"

static void print_byte(uint8_t b, FILE *f) {
  if (use_colors (f)) {
    if (b == 0xff) fprintf (f, RED);
    if (b == 0x00) fprintf (f, GREEN);
    if (isprint (b)) fprintf (f, YELLOW);
  }
  fprintf (f, "%02hhx", b);
  if (use_colors (f)) {
    fprintf (f, CLEAR);
  }
}

#define WIDTH 16
#define BASE 0x0000

gender get_gender(Genotype *g) {
  return g->dna[0] % 2;
}

void set_gender(Genotype *g, gender gdr) {
  g->dna[0] = (g->dna[0] ^ NO_GENDER_MASK) | gdr;
}

static char get_gender_name(Genotype *g) {
  return "MF"[get_gender (g)];
}

void print_genotype(Genotype *g, FILE *f) {
  size_t i, j, k;
  int idx, s;
  uint8_t b;
  long unsigned int base = BASE;
  gene *usable_dna = get_usable_dna (g, &s);
  
  fprintf (f, "Genotype (length=%d, fitness={ hits=%d, value=%g }, gender=%c)\n",
	   dna_size (g), g->fitness.hits, g->fitness.value, get_gender_name (g));
  fprintf (f, "\tHexdump:\n");
  k = s / WIDTH;
  if ((s % WIDTH) > 0) k++;
  
  for (i = base - BASE; i < k; i++, base += WIDTH) {
    fprintf (f, "\t  0x%"PRIx64": ", base);
    
    for (j = 0; j < WIDTH; j++) {
      idx = j + (i * WIDTH);
      if (idx < s) print_byte (usable_dna[idx], f);
      else fprintf (f, "  ");
      fprintf (f, " ");
    }
    fprintf (f, "| ");
    for (j = 0; j < WIDTH; j++) {
      idx = j + (i * WIDTH);
      if (idx < s) {
	b = usable_dna[idx];
	if (use_colors (f)) {
	  if (b == 0xff) fprintf (f, RED);
	  if (b == 0x00) fprintf (f, GREEN);
	  if (isprint (b)) fprintf (f, YELLOW);
	}
	fprintf (f, "%c", (isprint (b)) ? b : '.');
	if (use_colors (f)) fprintf (f, CLEAR);
      } else {
	fprintf (f, " ");
      }
    }
    fprintf (f, "\n");
    
  }
}
