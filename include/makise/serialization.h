#pragma once

#ifndef _SERIALIZATION_H
#define _SERIALIZATION_H

#include <parameters.h>
#include <ga.h>

int serialize_program_state_to_file(Parameters *, Problem *, char *, bool);
int serialize_program_state(Parameters *, Problem *, void **, bool);
int deserialize_program_state(Parameters **, Problem **, void *, int);

#endif
