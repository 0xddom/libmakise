#pragma once

#ifndef _GEP_FUNCTIONS_H
#define _GEP_FUNCTIONS_H

#include <gep/tree.h>

#define op(n) void n(Tree *, void *)

op (add_op);
op (sub_op);
op (mul_op);
op (var);
op (nop);
op (adf);

#endif
