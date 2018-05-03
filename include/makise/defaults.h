#pragma once

#ifndef _DEFAULTS_H
#define _DEFAULTS_H

#include <mutation.h>
#include <crossover.h>

#define DEFAULT_POPULATION 100
#define DEFAULT_MUTATION bit_flip
#define DEFAULT_CROSSOVER one_point
#define DEFAULT_SEED time (0)
#define DEFAULT_DNA_LEN 100
#define DEFAULT_GENERATIONS 50
#define DEFAULT_TOURNAMENT 7
#define DEFAULT_MUTATION_RATE 0.02
#define DEFAULT_OUTPUT stdout
#define DEFAULT_LOGGER plain_text_log

#endif
