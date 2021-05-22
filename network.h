/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "layer.h"
#include "neuron.h"
#include <stddef.h>

struct network_st {
    char* name;
    layer_st** layers;
    size_t num_layers;
};

extern void network_init(network_st* nw, char* name);
extern void network_push_layer(network_st* nw, layer_st* layer);
extern void network_print(network_st* nw);
extern void network_free(network_st* nw);

extern void network_forward(network_st* nw, double* inputs, size_t size);
extern void network_backward(network_st* nw, double* targets, size_t size);
extern void network_update_weights(network_st* nw, double learningRate);

extern bool network_print_training(network_st* nw, double* targets, size_t size, size_t iter);

#endif /* __NETWORK_H__ */