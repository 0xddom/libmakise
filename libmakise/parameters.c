#include <parameters.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <defaults.h>
#include <mutation.h>
#include <crossover.h>
#include <errors.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

Parameters *init_params() {
  Parameters *params = (Parameters*)malloc (sizeof (Parameters));

  if (params == NULL) {
    fprintf (stderr, "Can't allocate space for the parameters\n");
    exit (ALLOC_ERROR);
  }
  
  params->population_size = DEFAULT_POPULATION;
  params->mutation_algo = DEFAULT_MUTATION;
  params->crossover_algo = DEFAULT_CROSSOVER;
  params->seed = DEFAULT_SEED;
  params->default_seed = true;
  params->dna_length = DEFAULT_DNA_LEN;
  params->generations = DEFAULT_GENERATIONS;
  params->tournament_size = DEFAULT_TOURNAMENT;
  params->mutation_rate = DEFAULT_MUTATION_RATE;
  params->output = DEFAULT_OUTPUT;
  params->logger = DEFAULT_LOGGER;
  params->use_csv = false;
  params->do_restore = false;
  params->restore_file = NULL;
  
  return params;
}

void help() {
  puts ("=-=-=-=- Makise genetic algorithm system help -=-=-=-=");
  puts ("Parameters");
  puts ("\t-p | --population\tThe size of the population");
  puts ("\t-m | --mutation\t\tThe mutation algorithm. See below.");
  puts ("\t-c | --crossover\tThe crossover algorithm. See below.");
  puts ("\t-d | --dna\t\tThe length of the dna buffer.");
  puts ("\t-s | --seed\t\tThe seed for the RNG. The default is the current time.");
  puts ("\t-g | --generations\tThe number of generations to run the algorithm.");
  puts ("\t-t | --tournament\tThe size of the tournament for selecction.");
  puts ("\t-r | --mutation-rate\tThe mutation rate.");
  puts ("\t-o | --output\t\tFile to write the report. - for stdout.");
  puts ("\t-h | --help\t\tShow this message and exit.");
  puts ("Available mutation algorithms");
  print_available_mutation_algorithms ();
  puts ("Available crossover algorithms");
  print_available_crossover_algorithms ();
}

#define SHORT_OPTIONS "p:m:c:d:s:g:ht:r:o:CR:"

Parameters *parse_parameters(int argc, char **argv) {
  Parameters *params;
  int c;
  int longidx;

  params = init_params ();
  params->prog_name = argv[0];
  
  struct option options[] = {
    { "population", optional_argument, NULL, 'p' },
    { "mutation", optional_argument, NULL, 'm' },
    { "crossover", optional_argument, NULL, 'c' },
    { "dna", optional_argument, NULL, 'd' },
    { "seed", optional_argument, NULL, 's' },
    { "generations", optional_argument, NULL, 'g' },
    { "help", no_argument, NULL, 'h' },
    { "tournament", optional_argument, NULL, 't' },
    { "mutation-rate", optional_argument, NULL, 'r' },
    { "output", optional_argument, NULL, 'o' },
    { "csv", no_argument, NULL, 'C' },
    { "restore", optional_argument, NULL, 'R' },
    { NULL, 0, NULL, 0 }
  };

  opterr = 0;

  while ((c = getopt_long (argc, argv, SHORT_OPTIONS, options, &longidx)) != -1) {
    switch (c) {
    case 'p':
      params->population_size = atoi (optarg);
      break;
    case 'm':
      params->mutation_algo = get_mutation_func (optarg);
      break;
    case 'c':
      params->crossover_algo = get_crossover_func (optarg);
      break;
    case 'd':
      params->dna_length = atoi (optarg);
      break;
    case 's':
      params->seed = atoi (optarg);
      params->default_seed = false;
      break;
    case 'g':
      params->generations = atoi (optarg);
      break;
    case 't':
      params->tournament_size = atoi (optarg);
      break;
    case 'r':
      params->mutation_rate = strtod (optarg, NULL);
      break;
    case 'o': {
      FILE *f;
      if (strcmp (optarg, "-") == 0) {
	f = stdout;
      } else {
	f = fopen (optarg, "w");
	if (f == NULL) {
	  fprintf (stderr, "Can't open the output file (%s): %s\n", optarg, strerror(errno));
	  exit (errno);
	}
      }
      params->output = f;
      break;
    }
    case 'C':
      params->logger = log_step_in_csv;
      params->use_csv = true;
      break;
    case 'R': {
      FILE *f;
      f = fopen (optarg, "r");
      if (f == NULL) {
	fprintf (stderr, "Can't open the restore file (%s): %s\n", optarg, strerror(errno));
	exit (errno);
      }
      
      params->restore_file = f;
      params->do_restore = true;
      break;
    }
    case 'h':
      help ();
      exit (0);
    case '?':
      if (isprint (optopt))
	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
	fprintf (stderr,
		 "Unknown option character `\\x%x'.\n",
		 optopt);
      exit (UNK_PARAM);
    default:
      abort ();
    }
  }

  return params;
}
