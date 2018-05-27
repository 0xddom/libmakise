#include <gep/tree.h>
#include <makise/errors.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static Sym *lookup_symbol(Sym *sym_table, char sym) {
  int i;

  for (i = 0; sym_table[i].sym != '\0'; i++) {
    if (sym_table[i].sym == sym) {
      return &(sym_table[i]);
    }
  }

  return NULL;
}

static char *compile_to_tree_rec(Tree *t, char *source, Sym *sym_table) {
  int i;
  char *rem;
  
  if (strlen (source) < 1) return "";
  
  Sym *sym = lookup_symbol (sym_table, *source);
  if (sym == NULL) {
    fprintf (stderr, "Unknown symbol %c found during compilation", *source);
    exit (UNK_PARAM);
  }

  t->sym = sym->sym;
  t->arity = sym->arity;
  t->op_func = (void*)sym->op_func;
  t->user_data = sym->extra;
  t->child = (Tree*)malloc (sizeof (Tree) * t->arity);
  if (t->child == NULL) {
    fprintf (stderr, "Can't allocate space for childs\n");
    exit (ALLOC_ERROR);
  }

  rem = source + 1;
  for (i = 0; i < t->arity; i++) {
    rem = compile_to_tree_rec (&t->child[i], rem, sym_table);
  }

  return rem;

}

Tree *compile_to_tree(char *source, Sym *sym_table) {
  Tree *t;
  
  t = (Tree*)malloc (sizeof (Tree));
  if (t == NULL) {
    fprintf (stderr, "Can't allocate space for a tree node\n");
    exit (ALLOC_ERROR);
  }

  compile_to_tree_rec (t, source, sym_table);

  return t;
}

void free_tree(Tree *t) {
  int i;
  
  for (i = 0; i < t->arity; i++) 
    free_tree (&t->child[i]);

  free (t->child);
  free (t);
}
