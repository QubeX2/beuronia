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
    char    *name;
    link_st **inputs;
    link_st **outputs;
    size_t num_inputs;
    size_t num_outputs;
    double bias;
    double total_input;
    double output;
    double output_derivate;
    double input_derivate;
    double acc_input_derivate;
    size_t num_acc_derivates;
    double (*func)(double);
};

extern void neuron_init(neuron_st *nn, char *name, double (*func)(double));
extern void neuron_push_link(neuron_st *nn, link_type_e type, link_st *link);
extern void neuron_update_output(neuron_st *nn);
extern void neuron_free(neuron_st *nn);
extern void neuron_print(neuron_st *nn);

#endif /* __NEURON_H__ */