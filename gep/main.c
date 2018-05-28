#include <makise.h>
#include <fenotype.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  double *vars;
  double result;
  Tree *adfs;
} ExecData;

#define ED(ptr) (*(ExecData*)(ptr))

void add_op(Tree *t, void *data) {
  eval_tree (&t->child[0], data);
  double result = ED(data).result;

  eval_tree (&t->child[1], data);
  ED(data).result = result + ED(data).result;
}

void sub_op(Tree *t, void *data) {
  eval_tree (&t->child[0], data);
  double result = ED(data).result;

  eval_tree (&t->child[1], data);
  ED(data).result = result - ED(data).result;
}

void mul_op(Tree *t, void *data) {
  eval_tree (&t->child[0], data);
  double result = ED(data).result;

  eval_tree (&t->child[1], data);
  ED(data).result = result * ED(data).result;
}

void var(Tree *t, void *data) {
  ED(data).result = ED(data).vars[((char)t->user_data) - 'a'];
}

void nop(Tree *t, void *data) {
  // Do nothing
}

#define EVALS 10

// The sum of all the squares of the vars
double target_function(double *vars, int len) {
  double sum = 0;

  for (int i = 0; i < len; i++) sum += vars[i] * vars[i];

  return sum;
}

void makise_eval_one_genotype(Genotype *g) {
  Fenotype *f;
  char ts[2048];
  double vars[3];

  f = genotype_to_fenotype (g);
  g->fitness.value = 0;
  g->fitness.hits = 0;

  for (int i = 0; i < EVALS; i++) {
    vars[0] = rand() % 1000;
    vars[1] = rand() % 1000;
    vars[2] = rand() % 1000;
  
    ExecData data = {
      .vars = vars,
      .result = 0.0,
      .adfs = NULL
    };
    
    //print_tree (f->tree, ts);
    //puts (ts);
    eval_tree (f->tree, (void*)&data);

    double expected = target_function (vars, 3);

    // Mirar en el codigo Ruby como se hacia la comparacion para la evaluacion.
    double diff = abs (expected - data.result);
    g->fitness.value += diff;
    if (diff <= 0.01) g->fitness.hits++;
    
    //printf ("%g - %g\n", data.result, expected);
  }

  g->fitness.value = 1 / (1 + g->fitness.value); // Normalized fitness
  //printf ("value=%g | hits=%d\n", g->fitness.value, g->fitness.hits);
  free_fenotype (f);

  //exit (0);
}


