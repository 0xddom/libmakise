#include <genotype.h>
#include <stdlib.h>
#include <errors.h>
#include <capstone/capstone.h>
#include <unistd.h>
#include <ctype.h>


Genotype *create_empty_genotype(int dna_length) {
  Genotype *g = (Genotype*)malloc (sizeof (Genotype));

  if (g == NULL) {
    fprintf (stderr, "Can't allocate space for a random genotype\n");
    exit (ALLOC_ERROR);
  }
  
  g->length = dna_length;
  g->evaluated = false;
  g->fitness.hits = 0;
  g->fitness.value = 0.0;

  g->dna = (uint8_t*)malloc (sizeof (char) * g->length );

  if (g->dna == NULL) {
    fprintf (stderr, "Can't allocate space for a dna segment\n");
    free (g);
    exit (ALLOC_ERROR);
  }

  return g;
}

Genotype *create_random_genotype(int dna_length) {
  int i;
  Genotype *g = create_empty_genotype (dna_length);

  for (i = 0; i < g->length; i++) {
    g->dna[i] = (uint8_t)rand ();
  }
  
  return g;
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
#define BASE 0x1000

void print_genotype(Genotype *g, FILE *f) {
  size_t i, j, k;
  csh handle;
  cs_insn *insn;
  int c, idx;
  uint8_t b;
  long unsigned int base = BASE;

  if ((c = cs_open (CS_ARCH_ARM, CS_MODE_ARM, &handle)) != CS_ERR_OK) {
    fprintf (stderr, "Can't load capstone");
    exit (c);
  }
  
  fprintf (f, "Genotype (length=%d, fitness={ hits=%d, value=%g })\n",
	   g->length, g->fitness.hits, g->fitness.value);
  i = cs_disasm (handle, g->dna, g->length, base, 0, &insn);
  if (i > 0) {
    fprintf (f, "\tCode:\n");
    for (j = 0; j < i; j++) {
      fprintf (f, "\t  0x%" PRIx64":\t", insn[j].address);

      for (k = 0; k < insn[j].size; k++)
	print_byte (insn[j].bytes[k], f);
      
      fprintf (f, "\t%-12s%s\n",
	       insn[j].mnemonic,
	       insn[j].op_str);
    }
    cs_free (insn, i);
  } else {
    fprintf (f, "\tCode: Can't disassemble.\n");
  }
  fprintf (f, "\tHexdump:\n");
  k = g->length / WIDTH;
  if ((g->length % WIDTH) > 0) k++;
  
  for (i = base - BASE; i < k; i++, base += WIDTH) {
    fprintf (f, "\t  0x%"PRIx64": ", base);
    
    for (j = 0; j < WIDTH; j++) {
      idx = j + (i * WIDTH);
      if (idx < g->length) print_byte (g->dna[idx], f);
      else fprintf (f, "  ");
      fprintf (f, " ");
    }
    fprintf (f, "| ");
    for (j = 0; j < WIDTH; j++) {
      idx = j + (i * WIDTH);
      if (idx < g->length) {
	b = g->dna[idx];
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
  
  cs_close (&handle);  
}
