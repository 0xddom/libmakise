#pragma once

#ifndef _GEP_TREE_H
#define _GEP_TREE_H

#include <stdbool.h>


typedef struct tree_node_t {
  int arity;
  char sym;
  void *op_func; // Because I can't define it recursively
  struct tree_node_t *child;
  void *user_data;
} Tree;

typedef void (*node_op_func)(Tree *, void *);

typedef struct {
  char sym;
  int arity;
  node_op_func op_func;
  void *extra;
} Sym;

Tree *compile_to_tree(char *, Sym *);
void free_tree(Tree *);

#endif
