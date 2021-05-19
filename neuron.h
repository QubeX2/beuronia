/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NEURON_H__
#define __NEURON_H__

#include <stddef.h>
#include <stdint.h>

typedef struct neuron_st neuron_st;

struct neuron_st {
    size_t size;
    double output;
    double bias;
    double *inputs;
    double *weights;
};

extern void neuron_init(neuron_st *n, size_t s);
extern double neuron_output(neuron_st *n);
extern void neuron_set_inputs(neuron_st *n, ...);
extern void neuron_set_weights(neuron_st *n, ...);


#endif /* __NEURON_H__ */