/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stddef.h>
#include "neuron.h"

typedef struct network_st {
    neuron_st **neurons;
    size_t num_neurons;
    size_t num_layers;
} network_st;

extern void network_init(network_st *nw);
extern void network_push_neuron(network_st *nw, neuron_st *n);
extern void network_print(network_st *nw);
extern void network_free(network_st *nw);

#endif /* __NETWORK_H__ */