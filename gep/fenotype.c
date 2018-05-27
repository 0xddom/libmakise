#include <fenotype.h>
#include <stdlib.h>

void add_op(Tree *t, void *data) {
}

void sub_op(Tree *t, void *data) {
}

void mul_op(Tree *t, void *data) {
}

void var_a(Tree *t, void *data) {
}

void var_b(Tree *t, void *data) {
}

void var_c(Tree *t, void *data) {
}

#define SYM_TABLE_LEN 6

static Sym sym_table_base[] = {
  { '+', 2, add_op, NULL },
  { '-', 2, sub_op, NULL },
  { '*', 2, mul_op, NULL },
  { 'a', 0, var_a, NULL },
  { 'b', 0, var_b, NULL },
  { 'c', 0, var_c, NULL }
};

static int print_tree(Tree *t, char *s) {
  int i, j;
  i = sprintf (s, "(%c", t->sym);
  if (t->arity > 0) i += sprintf (s + i, " ");
  for (j = 0; j < t->arity; j++) {
    i += print_tree (&t->child[j], s + i);
  }
  i += sprintf (s + i, ")");

  return i;
}

Fenotype *genotype_to_fenotype(Genotype *g) {
  Fenotype *f;
  char ts[2048];
  Sym *sym_table;

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

  gene *dna = get_usable_dna (g);

  // Traverse the dna to extract each cromosome
  // Convert the dna to the pre-order representation
  // Compile to a tree
  // Add to the fenotype
  
  f->tree = compile_to_tree ("+-aab", sym_table);

  print_tree (f->tree, ts);
  puts (ts);
  
  return f;
}

void free_fenotype(Fenotype *f) {
  free (f);
}

