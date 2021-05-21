/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#include <stdio.h>
#include "network.h"
#include "memory.h"

void network_print(network_st *nw)
{
    printf("Network: { .num_neurons=%zu }\n", nw->num_neurons);
}

void network_init(network_st *nw)
{
    nw->neurons = NULL;
    nw->num_neurons = 0;
    nw->num_layers = 0;
}

void network_push_neuron(network_st *nw, neuron_st *n)
{
    REALLOC(nw->neurons, nw->num_neurons + 1);
    nw->neurons[nw->num_neurons] = n;
    nw->num_neurons++;
}

void network_free(network_st *nw)
{
    FREE(nw->neurons);
}