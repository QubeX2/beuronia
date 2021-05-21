/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stddef.h>
#include "neuron.h"
#include "layer.h"

struct network_st {
    char *name;
    layer_st **layers;
    size_t num_layers;
};

extern void network_init(network_st *nw, char *name);
extern void network_push_layer(network_st *nw, layer_st *layer);
extern void network_print(network_st *nw);
extern void network_free(network_st *nw);

#endif /* __NETWORK_H__ */