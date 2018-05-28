#include <fenotype.h>
#include <stdlib.h>
#include <makise/errors.h>
#include <gep/functions.h>

#define SYM_TABLE_LEN 6
#define NON_TERMINALS 3
#define MAX_ARITY 2

static Sym sym_table_base[] = {
  { '+', 2, add_op, NULL },
  { '-', 2, sub_op, NULL },
  { '*', 2, mul_op, NULL },
  { 'a', 0, var, (void*)'a' },
  { 'b', 0, var, (void*)'b' },
  { 'c', 0, var, (void*)'c' }
  //  { 'N', 0, nop, NULL }
};

// tail = head * (max_arity - 1) + 1

Fenotype *genotype_to_fenotype(Genotype *g) {
  Fenotype *f;
  Sym *sym_table;
  int dna_size;
  char *code;

  f = (Fenotype*)malloc (sizeof (Fenotype));
  if (f == NULL) {
    fprintf (stderr, "Can't allocate space for fenotype\n");
    exit (ALLOC_ERROR);
  }

  f->genotype = g;

  sym_table = (Sym*)calloc (sizeof (Sym), g->n_cromosomes + SYM_TABLE_LEN);
  if (sym_table == NULL) {
    fprintf (stderr, "Can't allocate space for symbol table\n");
    exit (ALLOC_ERROR);
  }

  for (int i = 0; i < SYM_TABLE_LEN; i++) {
    sym_table[i] = sym_table_base[i];
  }

  gene *dna = get_usable_dna (g, &dna_size);

  f->adfs = (Tree**)malloc (sizeof (Tree *) * g->n_cromosomes);
  if (f->adfs == NULL) {
    fprintf (stderr, "Can't allocate space for the adfs\n");
    exit (ALLOC_ERROR);
  }

  code = (char*)calloc (sizeof (char), g->dna_length + 1);
  for (int i = 0; i < g->n_cromosomes - 1; i++) {
    for (int j = 0; j < g->dna_length; j++) {
      gene gen = dna[i * g->dna_length + j];
      int limit = (j >= g->head) ? NON_TERMINALS : SYM_TABLE_LEN;
      int offset = (j >= g->head) ? SYM_TABLE_LEN - NON_TERMINALS : 0;
      code[j] = sym_table[(gen % limit) + offset].sym;
    }
    f->adfs[i] = compile_to_tree (code, sym_table);
  }
  
  for (int j = 0, i = g->n_cromosomes - 1; j < g->dna_length; j++) {
    gene gen = dna[i * g->dna_length + j];
    int limit = (j >= g->head) ? NON_TERMINALS : SYM_TABLE_LEN;
    int offset = (j >= g->head) ? SYM_TABLE_LEN - NON_TERMINALS : 0;
    code[j] = sym_table[(gen % limit) + offset].sym;
  }
  
  f->tree = compile_to_tree (code, sym_table);
  free (code);

  return f;
}

void free_fenotype(Fenotype *f) {
  if (f->adfs != NULL) {
    for (int i = 0; i < f->genotype->n_cromosomes - 1; i++)
      if (f->adfs[i] != NULL) free (f->adfs[i]);
    free (f->adfs);
  }
  if (f->tree != NULL) free (f->tree);
  free (f);
}

