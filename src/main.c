#include <stdio.h>
#include <time.h>
#include <ga.h>
#include <mutation.h>
#include <crossover.h>

int main(int argc, char **argv) {
  Problem *p;
  
  p = init_problem (5, 10, time (0), print_genotype, bit_flip, xor_crossover);

  run_problem_up_to_generation (p, 1);
  free_problem (p);

  return 0;
}
