/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NEURON_H__
#define __NEURON_H__

#include <stddef.h>
#include "types.h"
#include "link.h"


struct neuron_st {
    link_st *inputs;
    link_st *outputs;
    size_t num_inputs;
    size_t num_outputs;
    double bias;
    double total_input;
    double output;
    double output_derivate;
    double input_derivate;
    double acc_input_derivate;
    double (*func)(double);
};

extern void neuron_init(neuron_st *n, double (*func)(double));
extern void neuron_add_inputs(neuron_st *n, ...);
extern void neuron_add_outputs(neuron_st *n, ...);
extern void neuron_update_output(neuron_st *n);
extern void neuron_free(neuron_st *n);

#endif /* __NEURON_H__ */