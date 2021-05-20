/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include "neuron.h"
#include "vmath.h"

void neuron_init(neuron_st *n, double (*func)(double))
{
    n->bias = VMATH_RANDF();
    n->func = func;
    n->inputs = NULL;
    n->outputs = NULL;
    n->num_inputs = 0;
    n->num_outputs = 0;
}

