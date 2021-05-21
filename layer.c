/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdio.h>
#include <string.h>
#include "layer.h"

void layer_print(layer_st *layer)
{
    printf("\tLayer: { .name=%s, .num_neurons=%zu }\n", layer->name, layer->num_neurons);
}

void layer_init(layer_st *layer, char *name)
{
    layer->name = strdup(name);
    layer->neurons = NULL;
    layer->num_neurons = 0;
}

void layer_push_neuron(layer_st *layer, neuron_st *n)
{
    REALLOC(layer->neurons, layer->num_neurons + 1);
    layer->neurons[layer->num_neurons] = n;
    layer->num_neurons++;
}

void layer_free(layer_st *layer)
{
    FREE(layer->neurons);
    FREE(layer->name);
}