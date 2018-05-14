#include <string.h>
#include <stdbool.h>
#include <serialization.h>
#include <defaults.h>
#include <genotype.h>
#include <stdlib.h>
#include <errors.h>
#include <time.h>

int serialize_program_state_to_file(Parameters *params, Problem *problem, char *path, bool use_last_good) {
  FILE *f;
  int err = 0;

  if ((f = fopen (path, "wb")) == NULL) {
    fprintf (stderr, "Can't open the state file. Exiting without saving...\n");
    exit (ALLOC_ERROR);
  }

  void *serialized_data;
  
  int siz = serialize_program_state (params, problem, &serialized_data, use_last_good);

  if (siz < 0) {
    fprintf (stderr, "Can't save the program state.\n");
    err = ALLOC_ERROR;
    goto cleanup;
  }

  if (fwrite (serialized_data, siz, 1, f) != 1) {
    fprintf (stderr, "Error while writing the state.\n");
    err = ALLOC_ERROR;
    goto cleanup;
  }

 cleanup:
  fclose (f);
  free (serialized_data);
  return err;
}


int calculate_buffer_size(Parameters *params, Problem *p) {
  int size =
    sizeof (int)    + // population_size
    sizeof (int)    + // seed
    sizeof (int)    + // dna_length
    sizeof (int)    + // generations
    sizeof (int)    + // current_generation
    sizeof (int)    + // tournament_size
    sizeof (bool)   + // csv
    sizeof (bool)   + // default_seed
    sizeof (double);  // mutation_rate

  const char *crossover_func_name = get_crossover_func_name (p->crossover_genotypes);
  size += strlen (crossover_func_name) + 1;
  const char *mutation_func_name = get_mutation_func_name (p->mutate_genotype);
  size += strlen (mutation_func_name) + 1;

  size += p->population_size *
    (4 * params->dna_length + // dna
     sizeof (bool)          + // evaluated
     sizeof (int)           + // hits
     sizeof (double));        // value
  
  return size; 
}

#define dump(t, f, s) memcpy (t, (f), s); t += s;

int serialize_program_state(Parameters *params, Problem *p, void **buf, bool use_last_good) {
  int buf_size = calculate_buffer_size (params, p);
  void *ptr;
  int i;
  Genotype **population;

  if (use_last_good) population = p->last_good_population;
  else population = p->population;
  
  *buf = (void*)malloc (buf_size);
  if (*buf == NULL) return -1;
  ptr = *buf;

  dump (ptr, &(p->population_size), sizeof (int));
  dump (ptr, &(params->seed), sizeof (int));
  dump (ptr, &(params->default_seed), sizeof (bool));
  dump (ptr, &(params->dna_length), sizeof (int));
  dump (ptr, &(params->generations), sizeof (int));
  dump (ptr, &(p->current_gen), sizeof (int));
  dump (ptr, &(p->tournament_size), sizeof (int));
  dump (ptr, &(params->use_csv), sizeof (bool));
  dump (ptr, &(p->mutation_rate), sizeof (double));

  const char *crossover_func_name = get_crossover_func_name (p->crossover_genotypes);
  dump (ptr, crossover_func_name, strlen (crossover_func_name) + 1);
  const char *mutation_func_name = get_mutation_func_name (p->mutate_genotype);
  dump (ptr, mutation_func_name, strlen (mutation_func_name) + 1);

  for (i = 0; i < p->population_size; i++) {
    Genotype *g = population[i];
    dump (ptr, &(g->dna), g->length);
    dump (ptr, &(g->evaluated), sizeof (bool));
    dump (ptr, &(g->fitness.hits), sizeof (int));
    dump (ptr, &(g->fitness.value), sizeof (double));
  }

  return buf_size;
}

#define load(f, t, s) memcpy ((t), (f), s); f += s;
#define TMP_BUF_SIZ 100

int deserialize_program_state(Parameters **params, Problem **p, void *buf, int size) {
  int old_population_size = (*p)->population_size;
  void *ptr;
  int i;
  char tmp[TMP_BUF_SIZ];
  Genotype **restored_population;

  ptr = buf;

  load (ptr, &(*p)->population_size, sizeof (int));
  (*params)->population_size = (*p)->population_size;
  restored_population = (Genotype **)malloc (sizeof (Genotype*) * (*p)->population_size);
  if (!restored_population) return -1;
  load (ptr, &(*params)->seed, sizeof (int));
  load (ptr, &(*params)->default_seed, sizeof (bool));
  if ((*params)->default_seed) (*params)->seed = DEFAULT_SEED; // If the last one was time(0), this one too
  load (ptr, &(*params)->dna_length, sizeof (int));
  load (ptr, &(*params)->generations, sizeof (int));
  load (ptr, &(*p)->current_gen, sizeof (int));
  load (ptr, &(*p)->tournament_size, sizeof (int));
  (*params)->tournament_size = (*p)->tournament_size;
  load (ptr, &(*params)->use_csv, sizeof (bool));
  if ((*params)->use_csv) (*params)->logger = log_step_in_csv;
  else (*params)->logger = DEFAULT_LOGGER;
  load (ptr, &(*p)->mutation_rate, sizeof (double));

  strncpy (tmp, ptr, TMP_BUF_SIZ);
  (*p)->crossover_genotypes = get_crossover_func (tmp);
  ptr += strlen (tmp) + 1;

  strncpy (tmp, ptr, TMP_BUF_SIZ);
  (*p)->mutate_genotype = get_mutation_func (tmp);
  ptr += strlen (tmp) + 1;
    
  if ((*p)->population) {
    free_population ((*p)->population, old_population_size);
  }
  (*p)->population = restored_population;
  (*p)->last_good_population = restored_population;
  for (i = 0; i < (*p)->population_size; i++) {
    int dna_length = sizeof (uint8_t) * (*params)->dna_length * 4;
    Genotype *g = (Genotype *)malloc (sizeof (Genotype));
    g->length = dna_length;
    uint8_t *dna = (uint8_t*)malloc (dna_length);
    load (ptr, dna, dna_length);
    g->dna = dna;
    load (ptr, &(g->evaluated), sizeof (bool));
    load (ptr, &(g->fitness.hits), sizeof (int));
    load (ptr, &(g->fitness.value), sizeof (double));

    (*p)->population[i] = g;
  }
  
  return 0;
}
