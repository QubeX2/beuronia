/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stddef.h>
#include <stdarg.h>
#include "neuron.h"
#include "memory.h"
#include "vmath.h"

void neuron_init(neuron_st *n, size_t s)
{
    n->size = s;
    n->bias = VMATH_RANDF();
    MALLOC(n->inputs, s);
    MALLOC(n->weights, s);
    for(size_t i=0;i<s;i++) {
        n->weights[i] = VMATH_RANDF();
    }
}

void neuron_free(neuron_st *n)
{
    FREE(n->inputs);
    FREE(n->weights);
    n->inputs = NULL;
    n->weights = NULL;
}

double neuron_output(neuron_st *n, double (*f)(double))
{
    return (*f)(vmath_dot(n->inputs, n->weights, n->size)) + n->bias;
}

void neuron_set_inputs(neuron_st *n, ...)
{
    va_list args;
    va_start(args, n);
    for(size_t i = 0; i < n->size; i++) {
        n->inputs[i] = va_arg(args, double);
    }
    va_end(args);
}

void neuron_set_weights(neuron_st *n, ...)
{
    va_list args;
    va_start(args, n);
    for(size_t i=0;i<n->size;i++) {
        n->weights[i] = va_arg(args, double);
    }
    va_end(args);
}
