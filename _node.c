/**
 * THIS FILE IS NOT IN USE ANYMORE
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stddef.h>
#include <stdarg.h>
#include "_node.h"
#include "memory.h"
#include "vmath.h"

void node_init(node_st *n, size_t s, double (*f)(double))
{
    n->size = s;
    n->bias = VMATH_RANDF();
    MALLOC(n->inputs, s);
    MALLOC(n->weights, s);
    for(size_t i=0;i<s;i++) {
        n->weights[i] = VMATH_RANDF();
    }
    n->func = f;
}

void node_free(node_st *n)
{
    FREE(n->inputs);
    FREE(n->weights);
    n->inputs = NULL;
    n->weights = NULL;
}

double node_predict(node_st *n, double learning_rate)
{
    n->dot = vmath_dot(n->inputs, n->weights, n->size) + n->bias;
    n->prediction = n->func(n->dot);
    n->error = vmath_error(n->prediction, n->target);
    n->d_error = vmath_d_error(n->prediction, n->target);
    n->d_prediction = vmath_d_afunc(n->dot, n->func);

    // partial derivatives
    double cost_w1 = n->d_error * n->d_prediction * n->inputs[0];
    double cost_w2 = n->d_error * n->d_prediction * n->inputs[1];
    double cost_b = n->d_error * n->d_prediction * 1;

    // change weights
    n->weights[0] -= learning_rate * cost_w1;
    n->weights[1] -= learning_rate * cost_w2;
    n->bias -= learning_rate * cost_b;
    return n->prediction;
}

double node_output(node_st *n, double (*f)(double))
{
    return (*f)(vmath_dot(n->inputs, n->weights, n->size)) + n->bias;
}

void node_set_inputs(node_st *n, ...)
{
    va_list ap;
    va_start(ap, n);
    for(size_t i = 0; i < n->size; i++) {
        n->inputs[i] = va_arg(ap, double);
    }
    va_end(ap);
}

void node_set_weights(node_st *n, ...)
{
    va_list ap;
    va_start(ap, n);
    for(size_t i=0;i<n->size;i++) {
        n->weights[i] = va_arg(ap, double);
    }
    va_end(ap);
}
