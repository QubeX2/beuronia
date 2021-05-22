/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */

#ifndef __LAYER_H__
#define __LAYER_H__

#include "memory.h"
#include "neuron.h"
#include <stddef.h>

struct layer_st {
    char* name;
    neuron_st** neurons;
    size_t num_neurons;
};

extern void layer_print(layer_st* layer);
extern void layer_init(layer_st* layer, char* name);
extern void layer_push_neuron(layer_st* layer, neuron_st* n);
extern void layer_free(layer_st* layer);
extern void layer_link_neurons(layer_st* lhlr, layer_st* rhlr, link_st* links);

#endif /* __LAYER_H__ */