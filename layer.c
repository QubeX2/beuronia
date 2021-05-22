/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include "layer.h"
#include <stdio.h>
#include <string.h>

void layer_link_neurons(layer_st* lhlr, layer_st* rhlr, link_st *links)
{
    char name[64];
    for(size_t i = 0; i < lhlr->num_neurons; i++) {
        neuron_st *lhnn = lhlr->neurons[i];
        for(size_t j = 0; j < rhlr->num_neurons; j++ ) {
            neuron_st *rhnn = rhlr->neurons[j];
            memset(&name, 0, ARRAY_LEN(name));
            sprintf(name, "link %zu: [%s] to [%s]", i, lhnn->name, rhnn->name);
            link_init(&links[i], name, lhnn, rhnn);
        }
    }
}

void layer_print(layer_st* layer)
{
    printf("\tLayer: { .name=%s, .num_neurons=%zu }\n", layer->name, layer->num_neurons);
}

void layer_init(layer_st* layer, char* name)
{
    layer->name = strdup(name);
    layer->neurons = NULL;
    layer->num_neurons = 0;
}

void layer_push_neuron(layer_st* layer, neuron_st* n)
{
    REALLOC(layer->neurons, layer->num_neurons + 1);
    layer->neurons[layer->num_neurons] = n;
    layer->num_neurons++;
}

void layer_free(layer_st* layer)
{
    FREE(layer->neurons);
    FREE(layer->name);
}