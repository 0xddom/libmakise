#pragma once

#ifndef _LOGGING_H
#define _LOGGING_H

#include <genotype.h>

#define logging_func(name) void name(Genotype *, int, FILE *)
typedef void (*log_step_func)(Genotype *, int, FILE *);

logging_func(log_step_in_csv);
logging_func(plain_text_log);
#endif
